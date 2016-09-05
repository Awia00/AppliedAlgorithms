import java.util.concurrent.*;

public class Program2 {

    @SuppressWarnings("unchecked")
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

        int cores = Runtime.getRuntime().availableProcessors();

        ExecutorService es = Executors.newCachedThreadPool();

        Future<?>[] tasks = (Future<?>[]) new Future[cores];

	for (int t=0; t<cores; t++) {
            final int a = t;
            tasks[t] = es.submit(() -> {
                for(int i=a*N/cores; i<(a+1)*N/cores; i++) {
                    for(int j=0; j<M; j++) {
                        for(int k=0; k<K; k++) {
                            C[i * N + j] += A[i * N + k] * B[k * K + j];
                        }
                    }
                }
            });
        }

        for(int i=0; i<cores;i++) {
            try {
                tasks[i].get();
            } catch (ExecutionException | InterruptedException e) { /*Ignore*/ }
        }

        es.shutdown();

        double sum = 0;

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                sum += C[i * N + j];
            }
        }

        System.out.println(sum);
    }
}
