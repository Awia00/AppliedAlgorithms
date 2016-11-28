import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.Map;

public class Graph<T> {
    private Map<T,Collection<T>> graph = new HashMap<>();

    public Collection<T> get(T key) {
        return graph.get(key);
    }

    public void addEdge(T first, T second) {
        graph.putIfAbsent(first, new ArrayList<>());
        graph.putIfAbsent(second, new ArrayList<>());
        graph.get(first).add(second);
        graph.get(second).add(first);
    }

    public int size() {
        return graph.size();
    }

    public Collection<T> getVertices() {
        return graph.keySet();
    }
}
