{% extends "base.tpl" %}
{% block main %}
<h2>Log In</h2>
<form action="/login" method="POST">
	<input type="text" name="user" placeholder="login"><br/>
	<input type="password" name="pass" placeholder="pass"><br/>
	<input type="submit">
</form>
{% endblock %}
