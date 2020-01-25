{% extends "base.tpl" %}
{% block main %}
<h2>Sign up</h2>
<form action="/register" method="POST">
	<input type="text" name="user" placeholder="login"><br/>
	<input type="password" name="pass" placeholder="pass"><br/>
	<img src="{{ captcha }}"><br/>
	<input type="hidden" name="secret" value="{{ secret }}">
	<input type="text" name="answer" placeholder="captcha">
	<input type="submit">
</form>
{% endblock %}
