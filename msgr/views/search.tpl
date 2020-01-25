{% extends "base.tpl" %}
{% block main %}
<h2>searchContacts</h2>
<div class='roster'>
<form action='/search' method='GET'>
<input type='text' name='q'>
<input type='submit'>
</form>
{% if contacts %}
{% for user in contacts %}
	<div class='contact'><a href='/chat/{{ user }}'>{{ user }}</a></div>
{% endfor %}
{% else %}
<h3>Enter name to find user</h3>
{% endif %}
</div>
{% endblock %}
