import java.util.*;

public class Graph<T> {
    private Map<T,Set<T>> graph = new HashMap<>();

    public Iterable<T> get(T key) {
        return Collections.unmodifiableSet(graph.get(key));
    }

    public void addEdge(T first, T second) {
        if (!graph.containsKey(first)) graph.put(first, new HashSet<>());
        if (!graph.containsKey(second)) graph.put(second, new HashSet<>());
        graph.get(first).add(second);
        graph.get(second).add(first);
    }

    public int size() {
        return graph.size();
    }

    public Iterable<T> getVertices() {
        return Collections.unmodifiableSet(graph.keySet());
    }
}
