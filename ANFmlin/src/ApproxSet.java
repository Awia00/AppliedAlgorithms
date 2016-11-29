/**
 * Maintain a dynamic set under insertion, reporting the approximate number of distinct items inserted using a HyperLogLog counter.
 *
 * @author Rasmus Pagh
 * @version 2016.11.21
 */

public final class ApproxSet<T> {

    private static final int logm = 10;
    private static final int m = 1 << logm;
    private final byte[] M;

    public ApproxSet() {
        M = new byte[m];
    }

    public final void add(T x) {
        int xh = x.hashCode();
        if (xh != 0) {
            int i = LinearHash.f(xh, logm);
            byte val = (byte) ExpHash.hash(xh);
            if (val > M[i]) M[i] = val;
        }
    }

    public final void set(ApproxSet<T> from) {
        System.arraycopy(from.M, 0, M, 0, m);
    }

    public final int sizeEstimate() {
        double wsum = 0;
        int zerosum = 0;
        for (int j = 0; j < m; j++) {
            wsum += 2.0 / (2 << M[j]);
            if (M[j] == 0) zerosum++;
        }
        double Z = 1 / wsum;
        double estimate = m * m * Z * 0.7213 / (1 + 1.079 / m);
        if ((estimate < 2.5 * m) && (zerosum > 0))
            estimate = m * Math.log((double) m / zerosum);
        return (int) estimate;
    }

    public final void addSet(ApproxSet a) {
        for (int i = 0; i < M.length >> 4; i++) {
            max16(a.M, i << 4);
        }
    }


    private void max16(byte[] aM, int i) {
        if (M[i   ] < aM[i   ]) M[i   ] = aM[i   ];
        if (M[i+1 ] < aM[i+1 ]) M[i+1 ] = aM[i+1 ];
        if (M[i+2 ] < aM[i+2 ]) M[i+2 ] = aM[i+2 ];
        if (M[i+3 ] < aM[i+3 ]) M[i+3 ] = aM[i+3 ];
        if (M[i+4 ] < aM[i+4 ]) M[i+4 ] = aM[i+4 ];
        if (M[i+5 ] < aM[i+5 ]) M[i+5 ] = aM[i+5 ];
        if (M[i+6 ] < aM[i+6 ]) M[i+6 ] = aM[i+6 ];
        if (M[i+7 ] < aM[i+7 ]) M[i+7 ] = aM[i+7 ];
        if (M[i+8 ] < aM[i+8 ]) M[i+8 ] = aM[i+8 ];
        if (M[i+9 ] < aM[i+9 ]) M[i+9 ] = aM[i+9 ];
        if (M[i+10] < aM[i+10]) M[i+10] = aM[i+10];
        if (M[i+11] < aM[i+11]) M[i+11] = aM[i+11];
        if (M[i+12] < aM[i+12]) M[i+12] = aM[i+12];
        if (M[i+13] < aM[i+13]) M[i+13] = aM[i+13];
        if (M[i+14] < aM[i+14]) M[i+14] = aM[i+14];
        if (M[i+15] < aM[i+15]) M[i+15] = aM[i+15];
    }
}
