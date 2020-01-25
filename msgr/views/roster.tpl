{% extends "base.tpl" %}
{% block main %}
<h2>myContacts</h2>
<div class='roster'>
{% for user in contacts %}
	<div class='contact'><a href='/chat/{{ user }}'>{{ user }}</a></div>
{% endfor %}
</div>
{% endblock %}
