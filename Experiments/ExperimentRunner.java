import java.util.function.*;
import java.lang.ProcessBuilder;
import java.io.*;
import java.util.Arrays;

public class ExperimentRunner
{
    private ProcessBuilder processBuilder;
    private String program;
    private int argAmt, chosenVariableIndex;
    private double skip;
    private String[] defaults;

    public ExperimentRunner(String program, int argAmt, int chosenVariableIndex, double skip, String[] defaults)
    {
        this.program = program;
        this.chosenVariableIndex = chosenVariableIndex;
        this.defaults = defaults;        
        this.skip = skip;
    }

    private double stdev(double[] results, double mean)
    {
        double sdev = 0;
        for(int j = 0; j< results.length; j++)
        {
            sdev += (results[j] - mean) * (results[j] - mean);
        }
        return Math.sqrt(sdev/ results.length);
    }

    private void runExperiments(boolean skipTimes)
    {
        System.out.printf("%-40s %15s %12s\n", "Program with parameters", "mean", "standard dev");
        int count = 5;
        try{
            double runningTime = 0.0;
            int i = Integer.parseInt(defaults[chosenVariableIndex]);
            while(runningTime < 3)
            {
                defaults[chosenVariableIndex] = i + "";
                
                processBuilder = new ProcessBuilder(defaults);
                double st = 0;
                double[] stt = new double[count];
                for(int j = 0; j<count; j++)
                {
                    Timer t = new Timer();
                    {
                        Process process = processBuilder.start(); process.waitFor();
                    }
                    runningTime = t.check();
                    double time = runningTime; // microseconds
                    st += time;
                    stt[j] = time;
                }
                double mean = st/count;
                //System.out.printf("%-25s %15.1f us %10.2f %10d%n", Arrays.toString(defaults), mean, stdev(stt, mean), count); 
                System.out.printf("%-40s %15.4f %12.4f\n", Arrays.toString(defaults), mean, stdev(stt, mean));
                
                if(skipTimes)
                    i *= skip;
                else
                    i += skip;
            }
        } catch (Exception e)
        {
            System.out.println(e);
        }
    }

    private static class Timer {
        private long start, spent = 0;
        public Timer() { play(); }
        public double check() { return (System.nanoTime()-start+spent)/1e9; }
        public void pause() { spent += System.nanoTime()-start; }
        public void play() { start = System.nanoTime(); }
    }

    public static void main(String[] args)
    {
        if(args.length < 5)
        {
            System.out.println("You need to provide this program with the following parameters:");
            System.out.println("programPath, argAmt, chosenVariableIndex (1 indexed), skip, list of arguments to the program");
            System.exit(0);
        }
        String processName = args[0];
        int argAmt = Integer.parseInt(args[1]);
        int chosenVariableIndex = Integer.parseInt(args[2]);
        double skip = Double.parseDouble(args[3]);       

        String[] defaults = new String[argAmt + 1];
        defaults[0] = processName;
        for(int i = 0; i<argAmt; i++)
        {
            defaults[i+1] = args[i+4];
        }
        ExperimentRunner er = new ExperimentRunner(processName, argAmt, chosenVariableIndex, skip, defaults);
        er.runExperiments(false);
    }
}




