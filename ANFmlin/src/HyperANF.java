import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;


public class HyperANF {
    public static int calculateMedianDistance(Graph<String> g) {
        Map<String, ApproxSet<String>> c = new HashMap<>();

        // for v in V do:
        for (String v : g.getVertices()) {
            // initialize HyperLogLog counter c[v]
            ApproxSet<String> n = new ApproxSet<>();
            c.put(v, n);
            // add v to c[v]
            n.add(v);
        // end
        }

        // d := 0
        int d = 0;
        int reach;
        Map<String, ApproxSet<String>> m = new HashMap<>();

        // repeat
        do {
            // reach := 0
            reach = 0;

            // for v in V do:
            for (String v : g.getVertices()) {

                // initialize HyperLogLog counter m[v]
                ApproxSet<String> n = new ApproxSet<>();
                n.addSet(c.get(v));

                // for each w adjacent to v in E do:
                for (String w : g.get(v)) {
                    // m[v].AddSet(c[w])
                    n.addSet(c.get(w));
                // end
                }

                m.put(v, n);

                // reach := reach + size estimate from m[v]
                reach += n.sizeEstimate();
            // end
            }

            // for v in V do:
            for (String v : g.getVertices()) {
                // c[v] := m[v]
                c.put(v, m.get(v));
            // end
            }

            // d := d+1
            d++;

            // until reach >= |V|^2 / 2
        } while (reach < g.size() * g.size() / 2);

        // return d
        return d;
    }

    private static Graph<String> readGraph() {
        Scanner input = new Scanner(System.in);
        Graph<String> g = new Graph<>();
        while (input.hasNextLine()) {
            String[] tokens = input.nextLine().split(" ");
            String a = tokens[0];
            String b = tokens[1];

            g.addEdge(a, b);
        }
        return g;
    }

    public static void main(String[] args) throws FileNotFoundException {
        if (args.length > 0) {
            System.setIn(new FileInputStream(args[0]));
        }

        System.out.println(calculateMedianDistance(readGraph()));
    }
}
