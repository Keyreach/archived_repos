import java.util.regex.*;

public class RomanDigits implements Result {
    String Stroka;

    RomanDigits(String _Stroka){
        if(_Stroka.matches("^[IVXLCDMivxlcdm]*$")){
            this.Stroka = _Stroka.toUpperCase();
        } else {
            this.Stroka = null;
        }
    }

    @Override
    public String getResult() {
        int A = 0;
        char prev = 0;
        /*
        for(int i=0;i<Stroka.length();i++){
            if(i>0)
                prev = Stroka.charAt(i-1);
            switch(Stroka.charAt(i)){
                case 'I': A += 1; break;
                case 'V': if((prev == 'I')){A += 3;} else {A+=5;}; break;
                case 'X': if((prev == 'I')){A += 8;} else {A+=10;}; break;
                case 'L': if((prev == 'X')){A += 30;} else {A+=50;}; break;
                case 'C': if((prev == 'X')){A += 80;} else {A+=100;}; break;
                case 'D': if((prev == 'C')){A += 300;} else {A+=500;}; break;
                case 'M': if((prev == 'C')){A += 800;} else {A+=1000;}; break;
            }
        }
        */
        if(Stroka == null)
            return Integer.toString(A);
        for(int i=0;i<Stroka.length();i++){
            if(i>0)
                prev = Stroka.charAt(i-1);
            switch(Stroka.charAt(i)){
                case 'I':
                    A += 1;
                    break;
                case 'V':
                    A += 5;
                    switch(prev){
                        case 'I': A -= 2; break;
                    }
                    break;
                case 'X':
                    A += 10;
                    switch(prev){
                        case 'I': A -= 2; break;
                        case 'V': A -= 10; break;
                    }
                    break;
                case 'L':
                    A += 50;
                    switch(prev){
                        case 'I': A -= 2; break;
                        case 'V': A -= 10; break;
                        case 'X': A -= 20; break;
                    }
                    break;
                case 'C':
                    A += 100;
                    switch(prev){
                        case 'I': A -= 2; break;
                        case 'V': A -= 10; break;
                        case 'X': A -= 20; break;
                        case 'L': A -= 100; break;
                    }
                    break;
                case 'D':
                    A += 500;
                    switch(prev){
                        case 'I': A -= 2; break;
                        case 'V': A -= 10; break;
                        case 'X': A -= 20; break;
                        case 'L': A -= 100; break;
                        case 'C': A -= 200; break;
                    }
                    break;
                case 'M': A += 1000;
                    switch(prev){
                        case 'I': A -= 2; break;
                        case 'V': A -= 10; break;
                        case 'X': A -= 20; break;
                        case 'L': A -= 100; break;
                        case 'C': A -= 200; break;
                        case 'D': A -= 1000; break;
                    }
                    break;
            }
        }
        return Integer.toString(A);
    }
}
