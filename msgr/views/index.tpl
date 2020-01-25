{% extends "base.tpl" %}
{% block main %}
<h2>Write message</h2>
<form action='/send' method='POST'>
	<input type='text' name='address' {% if address %}value='{{ address }}'{% endif %}><br/>
	<textarea name='message'></textarea><br/>
	<input type='submit'>
</form>
{% endblock %}
