import java.util.*;
public class Main{
	
	private static void question1()
	{
		Scanner input = new Scanner(System.in);
		while(input.hasNextInt())
		{
			int inputInt = input.nextInt();
			System.out.println(inputInt);
		}
	}

	private static void question3()
	{
		Scanner input = new Scanner(System.in);
		while(input.hasNext())
		{
			String inputString = input.next();
			System.out.println(inputString);
		}
	}

	

	public static void main(String[] args)
	{
		question1();
		question3();
	}
}
