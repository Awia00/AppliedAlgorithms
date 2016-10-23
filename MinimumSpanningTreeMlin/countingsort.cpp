#include "graph.h"
#include "countingsort.h"

Edge *countingSort(Edge *edges, const int length, const int bucketNumber) {
    int *count = new int[bucketNumber];
    Edge *output = new Edge[length];

    for (int i = 0; i < length; i++) {
        count[edges[i].weight]++;
    }

    int total = 0;
    int oldCount;

    for (int i = 0; i < bucketNumber; i++) {
        oldCount = count[i];
        count[i] = total;
        total += oldCount;
    }

    for (int i = 0; i < length; i++) {
        Edge x = edges[i];
        output[count[x.weight]] = x;
        count[x.weight]++;
    }
    
    return output;
}