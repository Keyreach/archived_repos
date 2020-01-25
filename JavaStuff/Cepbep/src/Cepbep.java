import java.net.*;
import java.io.*;
import java.util.Date;
/**
 * Created by ake on 27.09.14.
 */
public class Cepbep {
    public static void main(String[] args){
        try {
            ServerSocket skt = new ServerSocket(12345);
            while(true){
                Socket incoming = skt.accept();
                BufferedReader in = new BufferedReader(new InputStreamReader(incoming.getInputStream()));
                PrintWriter out = new PrintWriter(new OutputStreamWriter(incoming.getOutputStream()));
                long now = System.currentTimeMillis()/1000L;
                System.out.println("["+now+"] Client has been connected.");
                out.println("Client has been connected. Type \"quit\" to disconnect.");
                out.flush();
                while(true){
                    String req = in.readLine();
                    if(req == null) {
                        break;
                    } else {
                        RomanDigits RD = new RomanDigits(req);
                        String resp = RD.getResult();
                        now = System.currentTimeMillis()/1000L;
                        System.out.println("["+now+"] Client has sent "+req);
                        System.out.println("["+now+"] Server has sent "+resp);
                        out.println(resp);
                        out.flush();
                        if(req.trim().equals("quit"))
                            break;
                    }
                }
                now = System.currentTimeMillis()/1000L;
                System.out.println("["+now+"] Client has been disconnected.");
                incoming.close();
            }
        } catch(Exception e){
            e.printStackTrace();
        }
    }
}
