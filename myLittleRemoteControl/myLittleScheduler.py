#!/usr/bin/python

import datetime
import threading

'''
	Defaults dictionary
'''

Defaults = {
	'priority': 50,
	'interval': 1.0,
}

class Job:
	def __init__(self, func, args, month=None, day=None, weekday=None, hour=None, minute=None, priority=Defaults['priority']):
		self.function = func
		self.arguments = args
		self.date = {
			'month': month,
			'day': day,
			'weekday': weekday,
			'hour': hour,
			'minute': minute
		}
		self.priority = priority
		self.status = 0

	def __str__(self):
		return self.date['month'] + ' ' + self.date['day'] + ' ' + self.date['weekday'] + ' ' + self.date['hour'] + ' ' + self.date['minute'] + ' ==> ' + ' '.join(self.arguments)

	def run(self):
		self.function(*(self.arguments))

class Store:
	def __init__(self):
		self.jobs = []
		self.running = True

	def add(self, job):
		self.jobs.append(job)

	def add_job(self, func, args, month=None, day=None, weekday=None, hour=None, minute=None, priority=Defaults['priority']):
		self.jobs.append(Job(func, args, month, day, weekday, hour, minute, priority))

	def stop(self):
		self.running = False
		
	def clear(self):
		self.jobs = []

	def run(self):
		if self.running:
			current = None
			threading.Timer(Defaults['interval'], self.run).start()
			now = datetime.datetime.now()
			for one in self.jobs:
				if (one.date['hour'] == str(now.hour) or one.date['hour'] == '*') and (one.date['minute'] == str(now.minute) or one.date['minute'] == '*') and (one.date['month'] == str(now.month) or one.date['month'] == '*') and (one.date['day'] == str(now.day) or one.date['day'] == '*') and (one.date['weekday'] == str(now.weekday()) or one.date['weekday'] == '*'):
					if one.status != 1 and now.second > 0:
						if current == None or one.priority > current.priority:
							current = one
						one.status = 1
					elif now.second == 59:
						one.status = 0
			if current:
				current.run()
