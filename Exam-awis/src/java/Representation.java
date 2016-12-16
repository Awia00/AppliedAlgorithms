import java.util.*;
public class Representation{
	
	private static void question3()
	{
		Scanner input = new Scanner(System.in);
		int n = input.nextInt();
		TreeMap<Integer, Integer> schedule = new TreeMap<>();

		for(int i = 0; i<n; i++)
		{
			double u = input.nextFloat(), v = input.nextFloat();
			int start = Math.ceil(u);
			int end = (int) v;
			for(int j = start; j++; j<=end)
			{
				schedule.put(j, i);
			}
		}

		while(!schedule.isEmpty())
		{
			
		}
	}
	

	public static void main(String[] args)
	{
		question3();
	}
}
