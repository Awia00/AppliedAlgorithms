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

	private static Map<String,Set<String>> graph = new HashMap<String,Set<String>>();
	private static int[] n;

	private static void readGraph() {
		Scanner input = new Scanner(System.in);
		while (input.hasNextLine()) {
			String line = input.nextLine();
			String a = line.split(" ")[0];
			String b = line.split(" ")[1];
			if (graph.get(a) == null) graph.put(a,new HashSet<String>());
			graph.get(a).add(b);
			if (graph.get(b) == null) graph.put(b,new HashSet<String>());
			graph.get(b).add(a);
		}
	}

	private static void BFS(String start) {
		Queue<String> queue = new ArrayDeque<String>();
		Set<String> visited = new HashSet<String>();
		
		visited.add(start);
		queue.add(start);
		int d = 0;
		while (queue.peek()!=null) {
			int newNodes = queue.size();
			n[d++] += newNodes;
			for (int i=0; i<newNodes; i++) {
				String v = queue.poll();
				for (String w : graph.get(v)) {
					if (!visited.contains(w)) {
						visited.add(w);
						queue.add(w);
					}
				}
			}
		}
	}
	
    public static void question1()
    {
        Scanner input = new Scanner(System.in);
        while(input.hasNextLine())
        {
            String line = input.nextLine();
            if(line.startsWith("//")) line = input.nextLine();
            List<Integer> line1 = Arrays.stream(line.split(" ")).map((String s) -> Integer.parseInt(s)).collect(Collectors.toList());
            List<Integer> line2 = Arrays.stream(input.nextLine().split(" ")).map((String s) -> Integer.parseInt(s)).collect(Collectors.toList());
            ApproxSet a = new ApproxSet();
            ApproxSet b = new ApproxSet();
            for(int i = 0; i<line1.size(); i++)
            {
                a.add(line1.get(i));
            }
            for(int i = 0; i<line2.size(); i++)
            {
                b.add(line2.get(i));
            }
            double estimateA = a.sizeEstimate();
            double estimateB = b.sizeEstimate();
            a.addSet(b);
            double estimateC = a.sizeEstimate();

            if(estimateC/estimateA-1 <= 0.2)
            {
                System.out.println("almost disjoint");
            }
            else if(estimateC/estimateA-1 >= 0.8)
            {
                System.out.println("almost same");
            }
        }
    }

	public static void main(String[] args) {
		question1();
	}
}