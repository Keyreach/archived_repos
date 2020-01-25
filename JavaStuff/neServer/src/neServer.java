import java.io.*;
import java.net.*;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
/**
 * Created by ake on 13.10.14.
 */
public class neServer {
    List<Student> SomeDB = new ArrayList<Student>();
    int port = 8080;
    ServerSocket SSckt;
    public class neThread extends Thread {
        Socket CSckt;
        public neThread(Socket KSckt){
            CSckt = KSckt;
        }
        public void run(){
            try {
                BufferedReader in = new BufferedReader(new InputStreamReader(CSckt.getInputStream()));
                PrintWriter out = new PrintWriter(new OutputStreamWriter(CSckt.getOutputStream()));
                System.out.println("Client has been connected.");
                String responseBody = "";
                String requestLine;
                String postBody = "";
                String httpRequest = "";
                String responseHeader;
                while((requestLine = in.readLine()).length() > 0) {
                    httpRequest += requestLine + '\n';
                }
                Pattern lengthPattern = Pattern.compile("Content-Length: ([1234567890]*)", Pattern.DOTALL);
                Matcher lengthMatcher = lengthPattern.matcher(httpRequest);
                if(lengthMatcher.find()) {
                    int postLen = Integer.parseInt(lengthMatcher.group(1));
                    for(int i=0; i<postLen; i++)
                        postBody += (char)in.read();

                }
                Pattern uriPattern = Pattern.compile("\\s\\/(.*?)\\s");
                Matcher uriMatcher = uriPattern.matcher(httpRequest);
                if(httpRequest.matches("(?s)^GET .*")) {
                    while(uriMatcher.find()) {
                        responseHeader = "HTTP/1.1 200 OK\nServer: neServer 0.1\n";
                        String uri = uriMatcher.group(1);
                        System.out.println(uri);
                        if(uri.matches("(?s).*\\.js")) {
                            responseHeader += "Content-Type: text/javascript\n\n";
                        } else if(uri.matches("(?s).*\\.css")) {
                            responseHeader += "Content-Type: text/css\n\n";
                        } else {
                            responseHeader += "Content-Type: text/html; charset=utf-8\n\n";
                        }
                        File reqfile = new File("/home/ake/.neserver/"+uri);
                        if(!reqfile.isDirectory() && reqfile.exists()) {
                            BufferedReader br = new BufferedReader(
                                    new InputStreamReader(
                                            new FileInputStream(reqfile), "UTF-8"
                                    )
                            );
                            String reqfilebuf = "";
                            String line = null;
                            while ((line = br.readLine()) != null) {
                                reqfilebuf += line + "\n";
                            }
                            br.close();
                            out.print(responseHeader+reqfilebuf);
                        } else {
                            out.print("HTTP/1.1 418 I'm a teapot\n" +
                                    "Server: neServer 0.1\nContent-Type: text/html; charset=utf-8\n" +
                                    "\n<html><body style='text-align:center'><h1>418 I'm a teapot</h1><p>neServer loves you %username% :3</p></body></html>");
                        }
                        out.flush();
                    }
                } else
                if(httpRequest.matches("(?s)^POST .*")) {
                    responseHeader = "HTTP/1.1 200 OK\nServer: neServer 0.1\nContent-Type: text/html; charset=utf-8\n\n";
                    if(uriMatcher.find()) {
                        if (uriMatcher.group(1).equals("numerals")) {
                            RomanDigits rd = new RomanDigits(postBody);
                            responseBody = rd.getResult();
                            out.print(responseHeader + responseBody); // Возвращаем ответ
                            out.flush();
                        } else {
                            String[] params = postBody.split("&");
                            Map<String, String> reqmap = new HashMap<String, String>();
                            for (String pair : params) {
                                reqmap.put(pair.split("=")[0], pair.split("=")[1]);
                            }
                            if (reqmap.get("action").equals("ls")) {
                                for(Student stud : SomeDB){
                                    responseBody += SomeDB.indexOf(stud) + "," + stud.getName() + "," + stud.getGroup() + ";";
                                }

                            } else if(reqmap.get("action").equals("rm")) {
                                if(!reqmap.get("id").isEmpty()) {
                                    int rm_id = Integer.parseInt(reqmap.get("id"));
                                    for(Student stud : SomeDB){
                                        if(SomeDB.indexOf(stud)==rm_id) {
                                            SomeDB.remove(rm_id);
                                            responseBody = "Student has been removed.";
                                        }
                                    }
                                }
                            } else if(reqmap.get("action").equals("ed")) {
                                if(!reqmap.get("id").isEmpty()&&!reqmap.get("name").isEmpty()&&!reqmap.get("group").isEmpty()) {
                                    int ed_id = Integer.parseInt(reqmap.get("id"));
                                    String ed_name = reqmap.get("name");
                                    String ed_group = reqmap.get("group");
                                    for(Student stud : SomeDB){
                                        if(SomeDB.indexOf(stud)==ed_id) {
                                            stud.setName(ed_name);
                                            stud.setGroup(ed_group);
                                            responseBody = "Student has been edited.";
                                        }
                                    }
                                }
                            } else if(reqmap.get("action").equals("mk")) {
                                if(!reqmap.get("name").isEmpty()&&!reqmap.get("group").isEmpty()) {
                                    String mk_name = reqmap.get("name");
                                    String mk_group = reqmap.get("group");
                                    SomeDB.add(new Student(mk_name, mk_group));
                                    responseBody = "Student has been created.";
                                }
                            } else {
                                    responseBody = "<h1>Thy request doesth not recognized</h1>";
                            }
                            out.print(responseHeader + responseBody);
                            out.flush();
                        }
                    }
                }
                CSckt.close(); // Закрываем подключение
                System.out.println("Client has been disconnected.");
            } catch(Exception e) {
                e.printStackTrace();
            }
        }
    }
    public neServer() {
        Socket CSckt;
        try {
            SomeDB.add(new Student("Butylkin","RI-12309"));
            SomeDB.add(new Student("Mochalkin","RI-12309"));
            SSckt = new ServerSocket(port); // Создается серверный сокет
            System.out.println("Server started");
        } catch(Exception e){
            e.printStackTrace();
        }
        while(true){
            try {
                CSckt = SSckt.accept();
                neThread someThread = new neThread(CSckt);
                someThread.start();
            } catch(Exception e) {
                e.printStackTrace();
            }
        }
    }
    public static void main(String[] args){
        neServer someServer = new neServer();
    }
}
