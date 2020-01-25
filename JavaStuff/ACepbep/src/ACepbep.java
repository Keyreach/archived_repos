import java.io.*;
import java.net.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Created by Admin on 29.09.14.
 */
public class ACepbep {
    int clients = 0;
    int port = 8080;
    ServerSocket SSckt;
    public class Session extends Thread {
        Socket CSckt;
        public Session(Socket KSckt){
            CSckt = KSckt;
        }
        public void run(){ // Переопределяем метод run(), который будет выполняться в побочном потоке
            try {
                BufferedReader in = new BufferedReader(new InputStreamReader(CSckt.getInputStream()));
                PrintWriter out = new PrintWriter(new OutputStreamWriter(CSckt.getOutputStream()));
                System.out.println("Client has been connected.");
                String resp2 = "", buf, req = in.readLine();
                    while(true) {
                        buf = in.readLine(); // Считываем строку из потока ввода в buf
                        if (buf.length() == 0) // Пустая строка - выход
                            break;
                        req += buf + '\n'; // Дописываем buf в переменную содержащую весь запрос
                    }
                    Pattern getre = Pattern.compile("^GET .*",Pattern.DOTALL); // Создаем регексп для поиска в запросе
                    Matcher getrem = getre.matcher(req);                      // использования метода GET
                    if(getrem.find()) {
                        Pattern P = Pattern.compile("\\s\\/(.*?)\\s"); // Создаем регексп для поиска URI
                        Matcher M = P.matcher(req);
                        while(M.find()) {
                            RomanDigits rd = new RomanDigits(M.group(1));
                            resp2 = "<h1 style='font:400 32pt sans-serif'>"+rd.getResult()+"</h1>"; // Получаем результат от метода getResult() объекта класса RomanDigits
                            //resp2 = "<h1>" + M.group(1) + "</h1>";
                        }
                        String resp = "HTTP/1.1 200 OK\nServer: 321Server\nContent-Type: text/html; charset=utf-8\n\n"; // Формируем заголовок HTTP ответа
                        out.print(resp + resp2); // Возвращаем ответ
                        out.flush();
                    }
                CSckt.close(); // Закрываем подключение
                System.out.println("Client has been disconnected.");
            } catch(Exception e) {
                e.printStackTrace();
            }
        }
    }
    public ACepbep() {
        Socket CSckt;
        try {
            SSckt = new ServerSocket(port); // Создается серверный сокет
            System.out.println("Server started");
        } catch(Exception e){
            e.printStackTrace();
        }
        while(true){
            try {
                CSckt = SSckt.accept(); // Ждём подключения
                Session someSession = new Session(CSckt); // Создаем объект класса Session, потомка класса Thread, в котором параллельно будет обрабатываться это подключение, передаем ему сокет
                someSession.start(); // и запускаем
            } catch(Exception e) {
                e.printStackTrace();
            }
        }
    }
    public static void main(String[] args){
        ACepbep someServer = new ACepbep();
    }
}
