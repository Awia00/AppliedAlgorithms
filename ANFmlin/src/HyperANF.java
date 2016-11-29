import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;


public class HyperANF {
    public static int calculateMedianDistance(Graph<String> g) {
        Map<String, ApproxSet<String>> c = new HashMap<>(g.size());
        Map<String, ApproxSet<String>> m = new HashMap<>(g.size());

        // for v in V do:
        for (String v : g.getVertices()) {
            m.put(v, new ApproxSet<>());
            // initialize HyperLogLog counter c[v]
            ApproxSet<String> n = new ApproxSet<>();
            c.put(v, n);
            // add v to c[v]
            n.add(v);
        // end
        }

        // d := 0
        int d = 0;
        final long limit = ((long) g.size()) * g.size() >> 1;
        long reach;
        boolean swapped = false;
        // repeat
        do {
            // reach := 0
            reach = 0;

            final Map<String, ApproxSet<String>> cc = swapped ? m : c;
            final Map<String, ApproxSet<String>> mm = swapped ? c : m;


            // for v in V do:
            for (String v : g.getVertices()) {
                // initialize HyperLogLog counter m[v]
                ApproxSet<String> n = mm.get(v);
                n.set(cc.get(v));

                // for each w adjacent to v in E do:
                for (String w : g.get(v)) {
                    // m[v].AddSet(c[w])
                    n.addSet(cc.get(w));
                // end
                }

                // reach := reach + size estimate from m[v]
                reach += n.sizeEstimate();
            // end
            }

            // for v in V do:
            //     c[v] := m[v];
            // end
            swapped = !swapped;

            // d := d+1
            d++;

            // until reach >= |V|^2 / 2
        } while (reach < limit);

        // return d
        return d;
    }

    private static Graph<String> readGraph(InputStream in) {
        Scanner input = new Scanner(in);
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
        System.out.println(calculateMedianDistance(readGraph(args.length > 0 ? new FileInputStream(args[0]) : System.in)));
    }
}
