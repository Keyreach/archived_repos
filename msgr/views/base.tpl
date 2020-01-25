<!doctype html>
<html>
	<head>
		<title>{{ page_title }}</title>
		<style>
			html, body {
				padding: 0;
				margin: 0;
			}
			* {
				box-sizing: border-box;
			}
			.bd {
				margin: 0 auto;
				width: 640px;
				background: #DDD;
				padding: 16px;
				font: 400 12pt Istok, sans-serif;
			}
			h2 {
				margin: 4px;
			}
			input {
				margin: 8px 0;
				width: 100%;
			}
			textarea {
				margin: 0;
				width: 100%;
				height: 120px;
			}
			form {
				margin: 0;
			}
			.roster, .dialog {
				padding: 8px;
				background: #fff;
				margin: 16px 0;
			}
			.reply, .contact {
				background: #d4dadd;
				padding: 8px;
				margin: 8px 0;
			}
		</style>
	</head>
	<body>
		<div class='bd'>
			<div style='float:right'>
				<a href="/">Home</a>
				<a href="/search">Search</a>
				<a href="/logout">Logout</a>
			</div>
			{% block main %}{% endblock %}
		</div>		
	</body>
</html>
