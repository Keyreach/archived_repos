import java.io.*;
import java.net.*;
import java.util.Scanner;
/**
 * Created by ake on 28.09.14.
 */
public class Klient {
    public static void main(String[] args){
        try {
            String host, str1, str2;
            Scanner scan = new Scanner(System.in);
            if(args.length > 0){
                host = args[0];
            } else {
                host = "localhost";
            }
            Socket skt = new Socket(host, 12345);
            BufferedReader in = new BufferedReader(new InputStreamReader(skt.getInputStream()));
            PrintWriter out = new PrintWriter(new OutputStreamWriter(skt.getOutputStream()));
            do {
                str2 = in.readLine();
                if (str2 == null) {
                    break;
                } else {
                    System.out.println("-> " + str2);
                }
                System.out.print("-> ");
                str1 = scan.nextLine();
                out.println(str1);
                out.flush();
            } while(!str1.trim().equals("quit"));
            out.println("bye");
            out.flush();
        } catch(Exception e) {
            e.printStackTrace();
        }
    }
}