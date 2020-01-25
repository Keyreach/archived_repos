import java.util.Scanner;

public class Laba {
    public static void main(String argc[] ) {
        Scanner scan = new Scanner(System.in);
        String Stroka = scan.nextLine();
        RomanDigits rd = new RomanDigits(Stroka);
        System.out.println(rd.getResult());
    }
}
