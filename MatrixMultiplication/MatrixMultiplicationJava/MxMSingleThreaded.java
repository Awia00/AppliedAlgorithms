public class MxMSingleThreaded {
    public static void main(String[] args) {
        int N = Integer.parseInt(args[0]);
        int M = args.length > 1 ? Integer.parseInt(args[1]) : N;
        int K = args.length > 2 ? Integer.parseInt(args[2]) : N;

        double[][] A, B, C;

        A = new double[N][K];

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < K; j++) {
                A[i][j] = 1 + i + j;
            }
        }

        B = new double[K][M];

        for (int i = 0; i < K; i++) {
            for (int j = 0; j < M; j++) {
                B[i][j] = 1 + i * j;
            }
        }

        C = new double[N][M];

        for(int i=0; i< N; i++)
            for(int j=0; j<M; j++)
                for(int k=0; k<K; k++)
                    C[i][j] += A[i][k] * B[k][j];

        double sum = 0;

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                sum += C[i][j];
            }
        }

        System.out.println(sum);
    }

}
