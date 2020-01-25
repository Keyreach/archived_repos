#!/usr/bin/python3

import threading
import os
import sys
from json import dumps, loads
from time import sleep
import requests
import myLittleScheduler

default_config = 'config.json'
setup = {
    'server': 'http://localhost',
    'state_file': 'mystate',
    'knock_timeout': 15,
    'debug_level': 1
}
stop  = False
state = {
    'id': 'Sample',
    'last': 0,
    'current': 0
}

class internal_scheduler(object):
    """
        Additional tracking for task scheduler
    """
    callstack = []
    stackpointer = -1
    def __init__(self):
        self.callstack = []
        self.stackpointer = -1
    def push(self, command):
        """
            Adds new scheduled task to tracking stack
        """
        self.callstack.append(command)
        self.stackpointer += 1
        return self.stackpointer
    def pull(self, index):
        """
            Removes completed task from tracking stack
        """
        if index == self.stackpointer:
            self.callstack.pop()
            self.stackpointer -= 1
            if self.stackpointer != -1:
                while self.callstack[self.stackpointer] is None:
                    self.stackpointer -= 1
                executor(self.callstack[self.stackpointer])
        else:
            self.callstack[index] = None

def logger_wrap(text, level):
    """
        Logger wrapper
    """
    if level > setup['debug_level']:
        print(' * ' + text)


def executor(command):
    """
        Task execute wrapper
    """
    print(' > ' + command)

def task_execute(task):
    """
        Schedule or execute task
    """
#    global tasks_pointer
    args = task.split(' ')
    if len(args) > 1:
#        tasks_stack.append(args[0])
#        tasks_pointer += 1
        ptr = task_scheduler.push(args[0])
        hour, minute = args[1].split(':')
        scheduler.add_job(logger_wrap, (args[0], 5), hour=hour, minute=minute)
        hour, minute = args[2].split(':')
        scheduler.add_job(task_scheduler.pull, (ptr,), hour=hour, minute=minute)
    else:
        executor(args[0])

def task_queue(tasks):
    """
        Sequently evaluates tasks
        Updates state
    """
    logger_wrap('new tasks', 2)
    for task in tasks:
        task_execute(task['command'])
        state['current'] = task['id']

def knocker():
    """
        Knocks server
        Fetch new tasks and put them to queue if exist
    """
    while not stop:
        r = requests.post(setup['server'] + '/api/knock', json=state)
        logger_wrap('got ' + str(r.status_code), 1)
        resp = r.json()
        last = int(resp['id'])
        if last > state['last']:
            r = requests.post(setup['server'] + '/api/fetch', json=state)
            resp = r.json()
            state['last'] = last
            task_queue(resp)
        sleep(setup['knock_timeout'])

if __name__ == '__main__':
    if len(sys.argv) > 1:
        config = sys.argv[1]
    else:
        config = default_config
    if os.path.exists(config):
        logger_wrap('loading settings', 5)
        with open(config, 'r') as f:
            setup = loads(f.read())
        setup['debug_level'] = int(setup['debug_level'])
        setup['knock_timeout'] = int(setup['knock_timeout'])
    else:
        logger_wrap('using defaults', 5)
    if os.path.exists(setup['state_file']):
        with open(setup['state_file'], 'r') as f:
            statedump = f.read()
        if len(statedump) > 0:
            state = loads(statedump)
            if state['last'] > state['current']:
                state['last'] = state['current']
    if 'id' in setup:
        state['id'] = setup['id']
    scheduler = myLittleScheduler.Store()
    scheduler.run()
    task_scheduler = internal_scheduler()
    try:
        knock_thread = threading.Thread(target=knocker)
        knock_thread.start()
        while True:
            sleep(100)
    except(KeyboardInterrupt, SystemExit):
        logger_wrap('quitting', 5)
        with open(setup['state_file'], 'w') as f:
            f.write(dumps(state))
        stop = True
