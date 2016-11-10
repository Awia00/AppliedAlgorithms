package dk.mikaellindemann.appalg.countdistinct;

import java.util.Scanner;

public class Main {
    private static final int BITS = 32;

    private static int[] a = {
            0x21ae4036, 0x32435171, 0xac3338cf, 0xea97b40c, 0x0e504b22, 0x9ff9a4ef, 0x111d014d, 0x934f3787, 0x6cd079bf,
            0x69db5c31, 0xdf3c28ed, 0x40daf2ad, 0x82a5891c, 0x4659c7b0, 0x73dc0ca8, 0xdad3aca2, 0x00c74c7e, 0x9a2521e2,
            0xf38eb6aa, 0x64711ab6, 0x5823150a, 0xd13a3a9a, 0x30a5aa04, 0x0fb9a1da, 0xef785119, 0xc9f0b067, 0x1e7dde42,
            0xdda4a7b2, 0x1a1c2640, 0x297c0633, 0x744edb48, 0x19adce93 };

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        int threshold = scanner.nextInt();

        double result = hyperLogLog(1024, scanner);

        if (result < threshold) {
            System.out.println("below");
        }
        else {
            System.out.println("above");
        }
    }

    private static int rho(int hx) {
        // ρ(h(x)) = min{i | h(x)k−i = 1}
        int rho = 0;
        for (int j = 1 << 30; j > 0; j >>= 1) {
            rho++;
            if ((j & hx) > 0) {
                return rho;
            }
        }
        return rho;
    }

    private static double hyperLogLog(int m, Scanner input) {
        int[] M = new int[m];

        // for i:=0 to m-1 do M[i]:=0
        for (int i = 0; i < m; i++) {
            M[i] = 0; // Standard for java.
        }

        // for i:=1 to n do
        //     j := f(y[i])
        //     M[j] := max(M[j],rho(h(y[i])))
        // end
        while (input.hasNextInt()) {
            int yi = input.nextInt();
            int j = f(yi);
            M[j] = Math.max(M[j], rho(h(yi)));
        }

        // Z := 1/(2^(-M[0])+...+2^(-M[m-1]))
        // V := |{i | M[i]=0|}|
        double Z = 0;
        double V = 0;
        for (int i = 0; i < m; i++) {
            Z+= (Math.pow(2, -M[i]));
            if (M[i] == 0) V++;
        }
        Z = 1/Z;

        // E := m*m*Z*0.7213/(1 + 1.079/m)
        double E = m*m*Z*0.7213/(1 + 1.079/m);

//        if (E < 2.5*m and V > 0) then E:= m * ln(m/V)
        if (E < 2.5*m && V > 0) {
            E = m * Math.log(m/V);
        }

//        return E
        return E;
    }

    private static int h(int x) {
        int res = 0;
        for (int i = 0; i < BITS; i++) {
            res += (Integer.bitCount(a[i] & x) & 1) << (31 - i);
        }
        return res;
    }

    private static int f(int x) {
        return ((x*0xbc164501) & 0x7fe00000) >> 21;
    }
}
