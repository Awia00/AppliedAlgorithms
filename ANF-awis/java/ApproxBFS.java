/**
 * Compute array where n[i] is number of ordered vertex pairs (v1, v2) at distance i
 * Takes undirected graph input from stdin with one edge per line: "nodeID1 nodeID2"
 * @author Rasmus Pagh
 * @version 2016.10.22
 * Usage: java AllBFS < graph.txt
 */

import java.util.*;
import java.util.stream.*; 

public class ApproxBFS {

	public static void question3()
	{
		Scanner input = new Scanner(System.in);
		Map<Integer, ApproxSet> c = new HashMap<>();
		Map<Integer, List<Integer>> friends = new HashMap<>();
		while(input.hasNextLine())
		{
			int a = input.nextInt(), b = input.nextInt();
			if(!c.containsKey(a))
			{
				ApproxSet n = new ApproxSet();
				n.add(a);
				c.put(a, n);
			}
			if(!c.containsKey(b))
			{
				ApproxSet n = new ApproxSet();
				n.add(b);
				c.put(b, n);
			}
			if(!friends.containsKey(a))
			{
				List<Integer> n = new ArrayList<>();
				friends.put(a,n);
			}
			if(!friends.containsKey(b))
			{
				List<Integer> n = new ArrayList<>();
				friends.put(b,n);
			}
			friends.get(a).add(b);
			friends.get(b).add(a);
			
			input.nextLine();
		}
		int d = 0;
		double reach;
		
		do
		{
			reach = 0;
			
			Map<Integer, ApproxSet> m = new HashMap<>();
			for(Integer key : c.keySet())
			{
				ApproxSet n = new ApproxSet();
				n.addSet(c.get(key));
				for(Integer friend : friends.get(key))
				{
					n.addSet(c.get(friend));
				}
				m.put(key, n);
				reach += n.sizeEstimate();
			}
			for(Integer key : c.keySet())
			{
				c.put(key, m.get(key));
			}
			d++;
			//System.out.println(reach);
		}while(reach < c.size()*c.size() / 2);
		System.out.println(d);
	}

    public static void question1()
    {
        Scanner input = new Scanner(System.in);
        // while(input.hasNextLine())
        // {
            String line = input.nextLine();
            if(line.startsWith("/")) line = input.nextLine();
            String[] line1 = 
				line.split(" "), 
				line2 = input.nextLine().split(" ");
            ApproxSet a = new ApproxSet(), b = new ApproxSet();
            for(int i = 0; i<line1.length; i++)
            {
                a.add(line1[i]);
				b.add(line2[i]);
            }
            double eA = a.sizeEstimate(), eB = b.sizeEstimate();
            a.addSet(b);
            double eC = a.sizeEstimate(), result = (eA+eB-eC)/eC;
			//System.out.println("a: "+ eA + " b: " + eB + " c: " + eC + " res: " + result);
            if(result <= 0.4)
            {
                System.out.println("almost disjoint");
            }
            else if(result >= 0.5)
            {
                System.out.println("almost same");
            }
        // }
    }

	public static void main(String[] args) {
		//question1();
		question3();
	}
}