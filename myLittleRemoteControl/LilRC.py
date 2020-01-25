#!/usr/bin/python

import threading
import myLittleScheduler
from time import sleep
from subprocess import Popen
import yaml
import os

currentBatch = None
nullOutput = open(os.devnull, 'w')

Preferences = {
	'common': {
		'polltime': 15
	},
	'actions': {
		'media': ['/usr/bin/mpv','--quiet'],
		'image': ['/usr/bin/feh']
	}
}

def myLittleLogger(text):
	print(' * ' + text)

def prepareTask(arguments):
	myLittleLogger('Preparing task')

def execTask(command, arguments):
	myLittleLogger('Executing task: ' + command)
	if command in Preferences['actions']:
		Popen(Preferences['actions'][command] + arguments, stdout=nullOutput, stderr=nullOutput)
	else:
		myLittleLogger('Failed to recognize command')
	
def loadBatch(tasks):
	myLittleLogger("Loaded batch #" + currentBatch) 
	Scheduler.clear()
	for task in tasks:
		prepareTask(task['arguments'])
		if 'cron' in task:
			month, day, weekday, hour, minute = task['cron'].split(' ')
			Scheduler.add_job(execTask, (task['command'], task['arguments']), month=month, day=day, weekday=weekday, hour=hour, minute=minute)
		else:
			execTask(task['command'], task['arguments'])

try:
	myLittleLogger('My Little Remote Control')
	if os.path.exists('prefs.yaml'):
		myLittleLogger("Loading preferences from config")
		with open('prefs.yaml', 'r') as f:
			Preferences = yaml.safe_load(f.read())
	else:
		myLittleLogger("Falling back to defaults")
	Scheduler = myLittleScheduler.Store()
	Scheduler.run()
	while True:
		with open('tasks.yaml', 'r') as f:
			taskdata = yaml.safe_load(f.read())
			if currentBatch != taskdata['lastmod']:
				currentBatch = taskdata['lastmod']
				loadBatch(taskdata['tasklist'])
		sleep(Preferences['common']['polltime'])
except KeyboardInterrupt:
	Scheduler.stop()
	myLittleLogger("Bye")
