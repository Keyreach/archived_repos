{% extends "base.tpl" %}
{% block main %}
<h2>Chat</h2>
<div class='dialog'>
<form action='/send' method='POST'>
	<input type='hidden' name='address' value='{{ participant }}'>
	<textarea name='message'></textarea><br/>
	<input type='submit'>
</form>
{% for reply in dialog %}
	<div class='reply'>
		<b>{{ reply['user_name'] }}</b>
		<div>{{ reply['msg_text'] }}</div>
	</div>
{% endfor %}
</div>
{% endblock %}
