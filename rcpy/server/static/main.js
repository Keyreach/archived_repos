var Elements = {}

var Preferences = {
	'DeadTimeout': 180,
	'UpdateInterval': 10
}

function AjaxRequest(method, url, data, callback){
    var x = new XMLHttpRequest();
    x.open(method, url);
    x.onreadystatechange = function(){
        if(x.readyState == 4 && x.status == 200){
			callback(x.responseText);
        }
    };
    if(data){
		x.setRequestHeader('Content-Type', 'application/json; charset=UTF-8');
        x.send(JSON.stringify(data));
	} else {
        x.send();
	}
}

function EasyFormat(template, data){
	var result = new String(template);
	for(var i = 0, n = data.length; i < n; i++){
		result = result.replace(new RegExp('\\{' + i + '\\}', 'g'), data[i]);
	}
	return result;
}

var PostLoad = {
	'workers': function(){
		Elements['list'] = document.getElementById('workerslist');
		MainUpdateList();
		setInterval(MainUpdateList, Preferences['UpdateInterval'] * 1000)
		
	},
	'tasks': function(){
		Elements['list'] = document.getElementById('taskslist');
		Elements['status'] = document.getElementById('workerstatus');
		Elements['taskedit'] = document.getElementById('taskcommand');
		WorkerUpdateInfo();
		setInterval(WorkerUpdateInfo, Preferences['UpdateInterval'] * 1000);
	}
};

function MainUpdateList(){
	AjaxRequest(
		'POST',
		'/api/man/list',
		null,
		function(s){
			var data = JSON.parse(s);
			Elements['list'].innerHTML = '';
			for(var i in data){
				var listitem = document.createElement('div');
				listitem.className = (data[i]['status'] < Preferences['DeadTimeout'] ? 'box-ok' : 'box-fail');
				var listitemlink = document.createElement('a');
				listitemlink.href = '/worker/' + data[i]['id'];
				listitemlink.innerHTML = data[i]['id'];
				listitem.appendChild(listitemlink);
				Elements['list'].appendChild(listitem);
			}
		}
	);
}

function WorkerUpdateInfo(){
	AjaxRequest(
		'POST',
		'/api/man/info',
		{'id': subjectid },
		function(s){
			var data = JSON.parse(s);
			Elements['status'].innerHTML = EasyFormat('<b>Worker:</b> {0}<br/><b>Current:</b> {1}', [data['id'], data['current']]);
			Elements['current'] = data['current'];
			WorkerUpdateList();
		}
	);
}

function WorkerUpdateList(){
	AjaxRequest(
		'POST',
		'/api/man/tasks',
		{'id': subjectid },
		function(s){
			var data = JSON.parse(s);
			Elements['list'].innerHTML = '';
			for(var i in data){
				var listitem = document.createElement('div');
				listitem.className = (data[i]['id'] <= Elements['current'] ? 'box-ok' : 'box-fail');
				listitem.innerHTML = EasyFormat('{0}: {1}', [data[i]['id'], data[i]['command']]);
				Elements['list'].appendChild(listitem);
			}
		}
	);
}

function WorkerAddTask(){
	AjaxRequest(
		'POST',
		'/api/man/addtask',
		{
			'worker': subjectid,
			'command': Elements['taskedit'].value
		},
		function(s){
			var data = JSON.parse(s);
			if(data['status'] == 'ok'){
				console.log('OK');
				Elements['taskedit'].value = '';
				WorkerUpdateList();
			} else {
				console.warn('Error');
			}
		}
	);
}

document.addEventListener('DOMContentLoaded', PostLoad[pageid]);
