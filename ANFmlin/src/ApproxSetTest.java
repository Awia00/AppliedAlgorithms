import java.util.Scanner;

public class ApproxSetTest {
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);

        //while (input.hasNextLine()) {
        String line = input.nextLine();
        while (input.hasNextLine() && (line.startsWith("/") || line.isEmpty()))
            line = input.nextLine();

        String[] set1 = line.split(" ");
        String[] set2 = input.nextLine().split(" ");

        ApproxSet<String> a = new ApproxSet<>(), b = new ApproxSet<>();

        for (int i = 0; i < set1.length; i++) {
            a.add(set1[i]);
            b.add(set2[i]);
        }

        int aEst = a.sizeEstimate();
        int bEst = b.sizeEstimate();
        a.addSet(b);
        double cEst = a.sizeEstimate();

        if ((aEst + bEst - cEst) / aEst <= 0.2) System.out.println("almost disjoint");
        if ((aEst + bEst - cEst) / aEst >= 0.6) System.out.println("almost same");
        //}
    }
}
