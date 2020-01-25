<!doctype html>
<html>
 <head>
  <meta charset='utf-8'>
  <title>Worker {{ worker }} tasks</title>
  <link rel='stylesheet' href='/s/main.css' type='text/css'>
  <script>
   var pageid = 'tasks';
   var subjectid = '{{ worker }}';
  </script>
  <script src='/s/main.js'></script>
 </head>
 <body>
  <div class='hbox' id='top'>
   <b> RCPY </b> |
   <a href='/'>Workers</a> |
   <a href='/test/'>Testing</a>
  </div>
  <div class='hbox' id='workerstatus'>
  </div>
  <div class='hbox'>
   <b>New task:</b>
   <input type='text' id='taskcommand'>
   <button onclick='WorkerAddTask()'>Submit</button>
  </div>
  <div class='hbox' id='taskslist'>
  </div>
 </body>
</html>
