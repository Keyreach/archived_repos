/**
 * Created by ake on 02.10.14.
 */
public class Start {
    public static void main(String argc[]){
        String Stroka = "(16+(16*16))";
        String[] Str = new String[Stroka.length()];
        String tmp = "";
        int sp = 0;
        for(int i=0; i<Stroka.length(); i++){
            if(tmp.equals("")){
                tmp = Stroka.substring(i,i+1);
            } else if((tmp.matches("\\d")&&(Stroka.substring(i,i+1).matches("\\d")))){
                tmp += Stroka.charAt(i);
            } else {
                Str[sp] = tmp;
                sp++;
                tmp = Stroka.substring(i,i+1);
            }
        }
        Str[sp] = tmp;
        sp++;
        String[] stack = new String[sp];
        int sp2 = 0;
        String out = "";
        for(int i = 0; i < sp; i++){
            System.out.println(Str[i] + " " + Integer.toString(i));
            if(Str[i].matches("\\d")){
                out += Str[i] + " ";
            } else if(Str[i].trim().equals("(")) {
                stack[sp2] = Str[i];
                sp2++;
            } else if(Str[i].trim().equals(")")) {
                while(!(stack[sp2].trim().equals("(")) && (sp2>0)){
                    String tmp2 = stack[sp2];
                    sp2--;
                    out += tmp2 + " ";
                }
                sp2--;
            } else {
                stack[sp2] = Str[i];
                sp2++;
            }
        }
        System.out.println(out);
    }
}
