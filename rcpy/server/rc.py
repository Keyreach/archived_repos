#!/usr/bin/python

import os
import sqlite3
import time
from json import dumps
from bottle import Bottle, run, TEMPLATE_PATH, jinja2_template as template, request, redirect, response, abort
from bottle.ext import sqlite

WORKDIR = '/var/www/remotepy/'
DATADIR = '/var/www/remotepy/'
DATAFILE = os.path.join(DATADIR, 'main.db')

app = Bottle()
dbplugin = sqlite.Plugin(dbfile=DATAFILE)
app.install(dbplugin)

dbconn = sqlite3.connect(DATAFILE)
dbcurs = dbconn.cursor()
dbcurs.execute('CREATE TABLE IF NOT EXISTS workers(id TEXT PRIMARY KEY NOT NULL, current TEXT NOT NULL, accessed INTEGER)')
dbcurs.execute('CREATE TABLE IF NOT EXISTS tasks(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, workerid TEXT NOT NULL, command TEXT NOT NULL, FOREIGN KEY(workerid) REFERENCES workers(id))')

TEMPLATE_PATH.append(os.path.join(WORKDIR, 'views'))

@app.route('/api/knock', method='POST')
def req_knock(db):
    now = int(time.time())
    workerid = request.json.get('id')
    workercurrent = request.json.get('current')
    if db.execute('select id from workers where id = ?', (workerid, )).fetchone():
        db.execute('update workers set accessed = ? where id = ?', (now, workerid))
        db.execute('update workers set current = ? where id = ?', (workercurrent, workerid))
    else:
        db.execute('insert into workers values(?, ?, ?)', (workerid, workercurrent, now))
    lasttask = db.execute('select id from tasks where workerid = ? order by id desc', (workerid, )).fetchone()
    response.content_type = 'application/json'
    if not lasttask:
        return dumps({'id': '0'})
    else:
        return dumps({'id': lasttask[0]})

@app.route('/api/fetch', method='POST')
def req_fetch(db):
    workerid = request.json.get('id')
    workerlast = request.json.get('last')
    bundle = db.execute('select id, command from tasks where workerid = ? and id > ?', (workerid, workerlast)).fetchall()
    response.content_type = 'application/json'
    report = list(map(lambda x: {'id': x[0], 'command': x[1]}, bundle))
    return dumps(report)

@app.route('/api/man/tasks', method='POST')
def man_tasks(db):
    workerid = request.json.get('id')
    bundle = db.execute('select id, command from tasks where workerid = ?', (workerid,)).fetchall()
    response.content_type = 'application/json'
    report = list(map(lambda x: {'id': x[0], 'command': x[1]}, bundle))
    return dumps(report)

@app.route('/api/man/info', method='POST')
def man_info(db):
    workerid = request.json.get('id')
    info = db.execute('select id, current from workers where id = ?', (workerid,)).fetchone()
    response.content_type = 'application/json'
    return dumps({'id': info[0], 'current': info[1]})

@app.route('/api/man/list', method='POST')
def man_list(db):
    now = int(time.time())
    workerlist = db.execute('select id, current, accessed from workers')
    response.content_type = 'application/json'
    report = list(map(lambda x: {'id': x[0], 'current': x[1], 'status': now - x[2]}, workerlist))
    return dumps(report)
    
@app.route('/api/man/addtask', method='POST')
def man_addtask(db):
    taskworker = request.json.get('worker')
    taskcommand = request.json.get('command')
    db.execute('insert into tasks(workerid, command) values(?, ?)', (taskworker, taskcommand))
    return dumps({'status': 'ok'})

## PAGES

@app.route('/')
def page_workers():
    return template('workers')

@app.route('/worker/<id>')
def page_workers(id):
    return template('tasks', worker = id)

application = app
