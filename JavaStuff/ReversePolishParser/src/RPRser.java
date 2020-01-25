/**
 * Created by ake on 02.10.14.
 */
public class RPRser implements Result {
    String[] Str;
    public RPRser(String Stroka){
        String tmp = "";
        for(int i=0; i<Stroka.length(); i++){
            if(tmp.equals("")){
                tmp = Stroka.substring(i,i+1);
            } else if((tmp.matches("\\d")&&(Stroka.substring(i,i+1).matches("\\d")))){
                tmp += Stroka.charAt(i);
            } else {
                Str[Str.length] = tmp;
                tmp = Stroka.substring(i,i+1);
            }
        }
        Str[Str.length] = tmp;
    }

    @Override
    public String getResult() {
        for(int i=0; i<Str.length; i++){
            System.out.println(Str[i]);
        }
        return "OK";
    }
}
