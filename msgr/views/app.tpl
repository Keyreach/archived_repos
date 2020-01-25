<!doctype html>
<html>
	<head>
		<title>MSGR webapp</title>
		<style>
			* {
				box-sizing: border-box;
			}
			body {
				margin:			0;
				padding:		28px 0 0 0;
				font:			400 12px "CatV 6x12 9";
				background:		#4E5257; /* #CCC; */
			}
			#hd {
				background:		#1E2227;
				color:			#FFFFFF;
				padding:		8px;
				width:			100%;
				height:			28px;
				position:		fixed;
				top:			0;
				left:			0;
			}
			#sb {
				background:		#2E3237;
				color:			#FFFFFF;
				position:		fixed;
				top:			28px;
				left:			0;
				width:			20%;
			}
			#sb .list-item {
				padding:		8px;
			}
			#sb .list-hilite {
				background:		#fff;
				color:			#000;
			}
			#bd {
				background:		#FFF;
				margin-left:	20%;
			}
			#dialog_top {
				padding:		12px;
				background:		#2E3237;
				color:			#fff;
			}
			#dialog_content {
				padding:		8px;
				margin-bottom:	90px;
			}
			#chat_form {
				padding:		8px;
				position:		fixed;
				bottom:			0;
				right:			0;
				left:			20%;
				background:		#fff;
			}
			.reply-smb {
				background:		#406080;
				padding:		8px;
				margin:			4px auto 4px 0;
				width:			80%;
				min-width:		200px;
			}
			.reply-me {
				background:		#408060;
				padding:		8px;
				margin:			4px 0 4px auto;
				width:			80%;
				min-width:		200px;
			}
			#hd button {
				font:			400 12px "CatV 6x12 9";
				padding:		0;
				background:		transparent;
				border:			0;
				border-bottom:	1px solid #fff;
				color:			#fff;
			}
			.dialog {
				position:		fixed;
				padding:		8px;
				background:		rgba(0, 0, 0, 0.7);
				color:			#fff;
				width:			240px;
				border:			1px solid #fff;
				display:		none;
			}
			.dialog input, .dialog button {
				display:		block;
				width:			100%;
				border:			1px solid #333;
				padding:		4px;
				background:		#fff;
				margin:			4px 0;
				font:			400 12px "CatV 6x12 9";
			}
			#sb input {
				background:		#1E2227;
				color:			#fff;
				font:			400 12px "CatV 6x12 9";
				border:			0;
				padding:		4px;
			}
		</style>
		<script>
			var contactlist;
			var msgr_state = {}
			var g = {}
			
			function httpGet(url){
				return new Promise(function(resolve, reject){
				var xhr = new XMLHttpRequest();
				xhr.open('GET', url, true);
				xhr.onload = function(){
					if (this.status == 200) {
						resolve(this.response);
					} else {
						var error = new Error(this.statusText);
						error.code = this.status;
						reject(error);
					}
				};
				xhr.onerror = function() {
					reject(new Error("Network Error"));
				};
				xhr.send();
				});
			}

			function httpPost(url, data){
				return new Promise(function(resolve, reject){
				var xhr = new XMLHttpRequest();
				xhr.open('POST', url, true);
				xhr.setRequestHeader('Content-Type', 'application/json');
				xhr.onload = function(){
					if (this.status == 200) {
						resolve(this.response);
					} else {
						var error = new Error(this.statusText);
						error.code = this.status;
						reject(error);
					}
				};
				xhr.onerror = function() {
					reject(new Error("Network Error"));
				};
				xhr.send(data);
				});
			}
			
			function ListVw(o){
				if(o){
					this.obj = o;
				} else {
					this.obj = document.createElement('div');
				}
				this.selected = -1;
				this.list = [];
				this.obj.className = 'list-view';
				this.render = function(x){
					return function(){
						x.obj.innerHTML = '';
						for(var i in x.list){
							var opt = x.new(x.list[i], i);
							x.obj.appendChild(opt);
						}
					}
				}(this);
				this.new = function(o){ 
					return function(x, i){
						var opt = document.createElement('div');
						opt.className = 'list-item';
						if(i == o.selected) opt.classList.add('list-hilite');
						o.item(opt, x);
						opt.onclick = function(lvw, ix){
							return function(){ lvw.set(ix); }
						}(o, i);
						return opt;
					}
				}(this);
				this.item = function(o, x){
					o.innerHTML = x;
				}
				this.set = function(x){
					if(this.selected != x)
						this.onchange(x, this);
					this.selected = x;
					this.render();
				}
				this.add = function(x){
					this.list.push(x);
					this.render();
				}
				this.remove = function(i){
					this.list.splice(i, 1);
					this.render();
				}
				this.onchange = function(x, self){
				}
			}
			
			function reload_contacts(){
				httpPost('/api/contacts', JSON.stringify({'auth_token': msgr_state.token})).then(function(x){
					var r = JSON.parse(x);
					if(r.status == 'fail'){
						alert('Insufficient privileges!');
					} else {
						contactlist.list = r.data;
						contactlist.render();
					}
				});
			}
			
			function reload_chat(user){
				httpPost('/api/chat/' + user, JSON.stringify({'auth_token': msgr_state.token})).then(function(x){
					var r = JSON.parse(x);
					if(r.status == 'fail'){
						alert('Insufficient privileges!');
					} else {
						msgr_state['dialog'] = user;
						g['dialog_top'].innerHTML = user;
						g['dialog_content'].innerHTML = '';
						for(var i in r.data){
							var reply = document.createElement('div');
							if(r.data[i].author == 'me')
								reply.className = 'reply-me';
							else
								reply.className = 'reply-smb';
							reply.innerHTML = r.data[i].message;
							g['dialog_content'].appendChild(reply);
							g['message_box'].value = '';
						}
					}
				});
			}
			
			function do_search(text){
				if(text == '')
					reload_contacts();
				else
					httpPost('/api/search', JSON.stringify({'auth_token': msgr_state.token, 'query': text})).then(function(x){
						var r = JSON.parse(x);
						if(r.status == 'fail'){
							alert('Insufficient privileges!');
						} else {
							contactlist.list = r.data;
							contactlist.render();
						}
					});
			}
			
			function do_authenticate(){
				var login = g['login_user'].value;
				var passw = g['login_pass'].value;
				httpPost('/api/authenticate', JSON.stringify({'user': login, 'pass': passw })).then(function(x){
					var r = JSON.parse(x);
					if(r.status == 'fail'){
						alert('Authentication error!');
						document.getElementById('login_pass').value = '';
					} else {
						msgr_state['token'] = r.data.auth_token;
						msgr_state['user'] = r.data.user;
						g['user_status'].innerHTML = 'Logged as ' + msgr_state['user'];
						g['btn_signin'].style.display = 'none';
						g['btn_logout'].style.display = 'inline-block';
						reload_contacts();
						localStorage.setItem('msgr_state', JSON.stringify(msgr_state));
						g['login_form'].style.display = 'none';
						g['contact_search'].disabled = false;
					}
				});
			}
			
			function do_logout(){
				httpPost('/api/logout', JSON.stringify({'auth_token': msgr_state.token})).then(function(x){
					localStorage.removeItem('msgr_state');
					g['btn_signin'].style.display = 'inline-block';
					g['btn_logout'].style.display = 'none';
					contactlist.list = ['Log in to view contact list'];
					contactlist.render();
					g['dialog_content'].innerHTML = 'SELECT CONTACT IN SIDEBAR TO VIEW CHAT';
					g['dialog_top'].innerHTML = '';
					g['user_status'].innerHTML = '';
					g['contact_search'].disabled = true;
				});
			}
			
			function do_signin(){
				g['login_form'].style.display = 'block';
				g['login_form'].style.top = (window.innerHeight - g['login_form'].offsetHeight) / 2 + 'px';
				g['login_form'].style.left = (window.innerWidth - g['login_form'].offsetWidth) / 2 + 'px';
			}
			
			function do_signup(){
				httpGet('/api/captcha').then(function(x){
					var r = JSON.parse(x);
					msgr_state['captcha_secret'] = r.data.secret;
					g['register_captcha'].src = r.data.captcha;
					g['register_user'].value = '';
					g['register_pass'].value = '';
					g['register_answer'].value = '';
				});
				g['register_form'].style.display = 'block';
				g['register_form'].style.top = (window.innerHeight - g['register_form'].offsetHeight) / 2 + 'px';
				g['register_form'].style.left = (window.innerWidth - g['register_form'].offsetWidth) / 2 + 'px';
			}
			
			function do_register(){
				var login = g['register_user'].value;
				var passw = g['register_pass'].value;
				var answer = g['register_answer'].value;
				httpPost('/api/register', JSON.stringify({'user': login, 'pass': passw, 'answer': answer, 'secret': msgr_state['captcha_secret'] })).then(function(x){
					var r = JSON.parse(x);
					if(r.status == 'fail'){
						alert('Registration error!');
						g['register_answer'].value = '';
						do_signup();
					} else {
						alert('Successfully registered!');
						g['register_form'].style.display = 'none';
					}
				});
			}
			
			function send_message(){
				var msg = g['message_box'].value;
				httpPost('/api/send', JSON.stringify({'auth_token': msgr_state.token, 'address': msgr_state.dialog, 'message': msg })).then(function(x){
					var r = JSON.parse(x);
					if(r.status == 'fail'){
						alert('Authentication error!');
					} else {
						reload_chat(msgr_state.dialog);
					}
				});
			}
			
			function adapt(){
				g['bd'].style.height = (window.innerHeight - g['bd'].offsetTop - 80) + 'px';
			}
			
			function pageready(){
				var cached_links = ['dialog_top', 'message_box', 'dialog_content', 'contact_list', 'login_form', 'login_user', 'login_pass', 'btn_signin', 'btn_logout', 'user_status', 'register_user', 'register_pass', 'register_form', 'register_captcha', 'register_answer', 'bd', 'contact_search'];
				for(var i in cached_links){
					g[cached_links[i]] = document.getElementById(cached_links[i]);
				}
				contactlist = new ListVw(g['contact_list']);
				contactlist.onchange = function(x, self){
					reload_chat(self.list[x]);
				}
				//contactlist.list = ['first contact', 'second contact', 'third contact', 'fourth contact'];
				contactlist.render();
				if(localStorage.getItem('msgr_state') !== null){
					msgr_state = JSON.parse(localStorage.getItem('msgr_state'));
					g['user_status'].innerHTML = 'Logged as ' + msgr_state['user'];
					g['btn_signin'].style.display = 'none';
					g['btn_logout'].style.display = 'inline-block';
					reload_contacts();
					g['contact_search'].disabled = false;
				}
				adapt();
			}
			
			document.addEventListener('DOMContentLoaded', pageready);
		</script>
	</head>
	<body>
		<div id='hd'>
			<u>MSGR</u>
			<span id='user_status'></span>
			<button onclick='do_signin()' id='btn_signin'>Sign In</button>
			<button onclick='do_signup()' id='btn_signup'>Sign Up</button>
			<button onclick='do_logout()' id='btn_logout'>Log Out</button>
		</div>
		<div id='sb'>
			<div class='list-item'>
				<input type='text' id='contact_search' placeholder='search' onkeyup='do_search(this.value)' style='width: 100%' disabled>
			</div>
			<div id='contact_list'></div>
		</div>
		<div id='bd'>
			<div id='dialog_top'></div>
			<div id='dialog_content'>SELECT CONTACT IN SIDEBAR TO VIEW CHAT</div>
		</div>
		<div id='chat_form'>
			<textarea id='message_box' style='height: 80px; width: 89%; vertical-align: top;'></textarea>
			<button onclick='send_message()' style='float:right; background: #08f; height: 80px; width: 10%; vertical-align: top; color: #fff; border: 0;'>SEND</button>
		</div>
		
		<div id='login_form' class='dialog'>
			<label for='login_user'>Login</label>
			<input type='text' id='login_user'>
			<label for='login_pass'>Password</label>
			<input type='password' id='login_pass'>
			<button onclick='do_authenticate()'>Log In</button>
		</div>
		
		<div id='register_form' class='dialog'>
			<label for='register_user'>Login</label>
			<input type='text' id='register_user'>
			<label for='register_pass'>Password</label>
			<input type='password' id='register_pass'>
			<img id='register_captcha'>
			<input type='type' id='register_answer'>
			<button onclick='do_register()'>Register</button>
		</div>
		
	</body>
</html>
