from bottle import Bottle, run, TEMPLATE_PATH, jinja2_template as template, request, redirect, response, default_app, abort, BaseRequest
import bottle_mysql
import os
import sys
import random
import MySQLdb
from PIL import Image, ImageDraw, ImageFont, ImageFilter
from json import dumps, loads
import base64
import io
import hashlib

BASEDIR = os.path.dirname(os.path.abspath(__file__))

TEMPLATE_PATH.append(os.path.join(BASEDIR, 'views/'))

app = Bottle()
plugin = bottle_mysql.Plugin(dbuser='webapp', dbpass='123456', dbname='msgr', keyword='db')
app.install(plugin)

def am_i_logged_in(req, db):
	sess = req.get_cookie("session_id")
	if not sess:
		return None
	db.execute('SELECT session_user FROM sessions WHERE session_id=%s', (sess, ))
	userinfo = db.fetchone()
	if userinfo:
		return userinfo['session_user']
	else:
		return None
		
def am_i_logged_in_api(token, db):
	db.execute('SELECT session_user FROM sessions WHERE session_id=%s', (token, ))
	userinfo = db.fetchone()
	if userinfo:
		return userinfo['session_user']
	else:
		return None
		
@app.route('/login', method='POST')
def login(db):
	session_id = random.getrandbits(32)
	username = request.forms.get('user')
	password = request.forms.get('pass')
	db.execute('SELECT user_id FROM users WHERE user_name=%s AND user_password=%s', (username, password))
	user_id = db.fetchone()
	if user_id:
		db.execute('INSERT INTO sessions(session_id, session_user) VALUES(%s, %s)', (session_id, user_id['user_id']))
		response.set_cookie("session_id", str(session_id))
	return redirect("/")
#	else:
#		abort(404)
		
def captcha_gen(text):
	image = Image.new('RGBA', (80, 28), (255,255,255,255))
	draw = ImageDraw.Draw(image)
	font = ImageFont.truetype(os.path.join(BASEDIR, "capfont.ttf"), 28)
	draw.text((0, 0), text, (0, 0, 0, 255), font=font)
	buffer = io.BytesIO()
	im1 = image.filter(ImageFilter.SMOOTH_MORE).filter(ImageFilter.CONTOUR)
	im1.save(buffer, format="PNG")
	img_str = base64.b64encode(buffer.getvalue())
	return 'data:image/png;base64,' + img_str.decode('utf-8')

@app.route('/logout')
def logout(db):
	response.set_cookie("session_id", "")
	return redirect("/")
	
@app.route('/signup')
def sign_up(db):
	secret = random.randint(10000, 99999)
	secret_hash = hashlib.md5(str(secret)).hexdigest()
	captcha = captcha_gen(str(secret))
	return template('register', captcha=captcha, secret=secret_hash)
	
@app.route('/register', method='POST')
def register(db):
	username = request.forms.get('user')
	password = request.forms.get('pass')
	answer = request.forms.get('answer')
	secrethash = request.forms.get('secret')
	if secrethash == hashlib.md5(answer).hexdigest():
		db.execute('INSERT INTO users(user_name, user_password, user_type) VALUES(%s, %s, 0)', (username, password))
		return redirect('/')
	else:
		return redirect('/signup')
		
@app.route('/search')
def usersearch(db):
	user = am_i_logged_in(request, db) 
	if user is None:
		return template("login", page_title="Login")
	else:
		query = request.query.get('q')
		if query:
			db.execute('SELECT user_name FROM users WHERE user_name LIKE %s', ('%' + query + '%', ))
			rows = db.fetchall();
			contacts = map(lambda x: x['user_name'], rows)
			return template("search", page_title="Searching", contacts=contacts)	
		else:
			return template("search", page_title="Searching")	


@app.route('/chat/<recipient>')
def userchat(db, recipient):
	user = am_i_logged_in(request, db)
	if user is None:
		return template("login", page_title="Login")
	else:
		db.execute('SELECT user_id FROM users WHERE user_name=%s', (recipient, ))
		participant = db.fetchone()['user_id']
		db.execute('''
		SELECT
		user_name, msg_text
		FROM
		messages
		JOIN
		users ON msg_sender = user_id
		WHERE
		(msg_sender = %s and msg_address = %s) or (msg_sender = %s and msg_address = %s)
		ORDER BY msg_id DESC
		''', (user, participant, participant, user))
		dialog = db.fetchall()
		return template("chat", page_title="Chat with " + recipient, dialog=dialog, participant=recipient)
		

@app.route('/send', method='POST')
def message_send(db):
	user = am_i_logged_in(request, db) 
	if user is None:
		return template("login", page_title="Not authorized")
	else:
		recipient = request.forms.get('address')
		message   = request.forms.get('message')
		db.execute('SELECT user_id FROM users WHERE user_name=%s', (recipient, ))
		msg_to = db.fetchone()
		if not msg_to:
			return template('error', page_title='Error', description='No such user')
		else:
			db.execute('INSERT INTO messages(msg_sender, msg_address, msg_text) VALUES(%s, %s, %s)', (user, msg_to['user_id'], message))
			return redirect('/chat/' + recipient)

# select
# contact
# from (
#	select msg_address as contact from messages where msg_sender = 1
#	union all
#	select msg_sender as contact from messages where msg_address = 1
# ) as t
# group by contact;

@app.route('/')
def roster(db):
	user = am_i_logged_in(request, db) 
	if user is None:
		return template("login", page_title="Not authorized")
	else:
		db.execute('''
SELECT
user_name
FROM (
	SELECT msg_address AS contact FROM messages WHERE msg_sender = %s
	UNION ALL
	SELECT msg_sender AS contact FROM messages WHERE msg_address = %s
) AS t
JOIN
users ON contact = user_id
GROUP BY user_name
		''', (user, user))
		contacts_table = db.fetchall();
		contacts = map(lambda x: x['user_name'], contacts_table)
		return template("roster", page_title="Contacts", contacts=contacts)
		
@app.route('/app')
def new_test():
	return template("app")

'''
	JSON API
'''

@app.route('/api/authenticate', method='POST')
def api_authenticate(db):
	"""
	:param user: Login
	:param pass: Password
	
	:return auth_token: Authentication token for further operations 
	"""
	session_id = random.getrandbits(32)
	username = request.json.get('user')
	password = request.json.get('pass')
	db.execute('SELECT user_id FROM users WHERE user_name=%s AND user_password=%s', (username, password))
	user_id = db.fetchone()
	if user_id:
		db.execute('INSERT INTO sessions(session_id, session_user) VALUES(%s, %s)', (session_id, user_id['user_id']))
		return dumps({ 'status': 'done', 'data': { 'user': username, 'auth_token': str(session_id) } })
	else:
		return dumps({ 'status': 'fail', 'description': 'Authentication failed' })
		
@app.route('/api/logout', method='POST')
def api_logout(db):
	"""
	:param auth_token: Authentication token
	"""
	auth_token = request.json.get('auth_token')
	db.execute('DELETE FROM sessions WHERE session_id = %s', (auth_token, ))
	
@app.route('/api/register', method='POST')
def api_register(db):
	"""
	:param user: Login
	:param pass: Password
	:param answer: Captcha answer
	:param secret: Captcha secret
	"""
	username = request.json.get('user')
	password = request.json.get('pass')
	answer = request.json.get('answer')
	secrethash = request.json.get('secret')
	if secrethash == hashlib.md5(answer.encode('utf-8')).hexdigest():
		db.execute('SELECT user_id FROM users WHERE user_name = %s', (username, ))
		existing_user = db.fetchone()
		if not existing_user:
			db.execute('INSERT INTO users(user_name, user_password, user_type) VALUES(%s, %s, 0)', (username, password))
			return dumps({ 'status': 'done', 'data': 'User ' + username + ' registered' })
		else:
			return dumps({ 'status': 'fail', 'description': 'Login ' + username + ' already registered' })
	else:
		return dumps({ 'status': 'fail', 'description': 'Verification failed' })
		
@app.route('/api/captcha')
def api_captcha():
	"""
	:return captcha: Captcha image as base64
	:return secret: Captcha secret
	"""
	secret = random.randint(10000, 99999)
	secret_hash = hashlib.md5(str(secret).encode('utf-8')).hexdigest()
	captcha = captcha_gen(str(secret))
	return dumps({ 'status': 'done', 'data': { 'captcha': captcha, 'secret': secret_hash } })

@app.route('/api/contacts', method='POST')
def api_dialogs(db):
	"""
	:param auth_token: Authentication token
	"""
	auth_token = request.json.get('auth_token')
	user = am_i_logged_in_api(auth_token, db)
	if user is None:
		return dumps({ 'status': 'fail', 'description': 'Authentication required' })
	else:
		db.execute('''
SELECT
user_name
FROM (
	SELECT msg_address AS contact FROM messages WHERE msg_sender = %s
	UNION ALL
	SELECT msg_sender AS contact FROM messages WHERE msg_address = %s
) AS t
JOIN
users ON contact = user_id
GROUP BY user_name
		''', (user, user))
		contacts_table = db.fetchall();
		contacts = map(lambda x: x['user_name'], contacts_table)
		return dumps({ 'status': 'done', 'data': list(contacts) })
		
@app.route('/api/chat/<recv>', method='POST')
def api_chat(db, recv):
	"""
	:param auth_token: Authentication token
	
	:return author: Message sender
	:return message: Message text
	"""
	auth_token = request.json.get('auth_token')
	user = am_i_logged_in_api(auth_token, db)
	if user is None:
		return dumps({ 'status': 'fail', 'description': 'Authentication required' })
	else:
		db.execute('SELECT user_id FROM users WHERE user_name=%s', (recv, ))
		participant = db.fetchone()['user_id']
		db.execute('''
		SELECT
		user_name, msg_text
		FROM
		messages
		JOIN
		users ON msg_sender = user_id
		WHERE
		(msg_sender = %s and msg_address = %s) or (msg_sender = %s and msg_address = %s)
		ORDER BY msg_id DESC
		''', (user, participant, participant, user))
		dialog = db.fetchall()
		messages = map(lambda x: { 'author': x['user_name'] if x['user_name'] == recv else 'me', 'message': x['msg_text'] }, dialog)
		return dumps({ 'status': 'done', 'data': list(messages) })
		
@app.route('/api/send', method='POST')
def api_send(db):
	"""
	:param address: Message recepient
	:param message: Message text
	"""
	auth_token = request.json.get('auth_token')
	user = am_i_logged_in_api(auth_token, db)
	if user is None:
		return dumps({ 'status': 'fail', 'description': 'Authentication required' })
	else:
		recipient = request.json.get('address')
		message   = request.json.get('message')
		db.execute('SELECT user_id FROM users WHERE user_name=%s', (recipient, ))
		msg_to = db.fetchone()
		if not msg_to:
			return dumps({ 'status': 'fail', 'description': 'User doesn\'t exist' })
		else:
			db.execute('INSERT INTO messages(msg_sender, msg_address, msg_text) VALUES(%s, %s, %s)', (user, msg_to['user_id'], message))
			return dumps({ 'status': 'done', 'data': 'Message sent' })
			
@app.route('/api/search', method='POST')
def api_search(db):
	"""
	:param auth_token: Authentication token
	
	:return author: Message sender
	:return message: Message text
	"""
	auth_token = request.json.get('auth_token')
	user = am_i_logged_in_api(auth_token, db)
	if user is None:
		return dumps({ 'status': 'fail', 'description': 'Authentication required' })
	else:
		query = request.json.get('query')
		if query:
			db.execute('SELECT user_name FROM users WHERE user_name LIKE %s', ('%' + query + '%', ))
			rows = db.fetchall();
			contacts = map(lambda x: x['user_name'], rows)
			return dumps({ 'status': 'done', 'data': list(contacts) })
		else:
			return dumps({ 'status': 'done', 'data': [] })	

if __name__ == '__main__':
	print(BASEDIR)
	run(app, host='localhost', port='12309', debug=True)
