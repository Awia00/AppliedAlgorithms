public class Program2 {
    public static void main(String[] args) {
        int N = Integer.parseInt(args[0]);
        int M = args.length > 1 ? Integer.parseInt(args[1]) : N;
        int K = args.length > 2 ? Integer.parseInt(args[2]) : N;

        double[] A, B, C;

        A = new double[N * K];

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < K; j++) {
                A[i * N + j] = 1 + i + j;
            }
        }

        B = new double[K * M];

        for (int i = 0; i < K; i++) {
            for (int j = 0; j < M; j++) {
                B[i * K + j] = 1 + i * j;
            }
        }

        C = new double[N * M];

        for(int i=0; i< N; i++)
            for(int j=0; j<M; j++)
                for(int k=0; k<K; k++)
                    C[i * N + j] += A[i * N + k] * B[k * K + j];

        double sum = 0;

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                sum += C[i * N + j];
            }
        }

        System.out.println(sum);
    }

}
