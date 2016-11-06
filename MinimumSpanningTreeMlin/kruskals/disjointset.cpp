#include "disjointset.h"

void DisjointSet::setUnion(long v1, long v2){
    long n1 = find(v1);
    long n2 = find(v2);

    if (n1 == n2) return;

    if (nodes[n1].rank < nodes[n2].rank) {
        // n2 has largest rank:
        nodes[n1].parent = n2;
    } else if (nodes[n2].rank < nodes[n1].rank) {
        // n1 has largest rank:
        nodes[n2].parent = n1;
    } else {
        nodes[n1].parent = n2;
        nodes[n2].rank++;
    }
}

DisjointSet::DisjointSet(long numVertices){
    nodes = new UF[numVertices];

    for(long i = 0; i < numVertices; i++) {
        nodes[i].parent=i;
    }
}

long DisjointSet::find(long id) {
    if (id == nodes[id].parent) return id;
    nodes[id].parent = find(nodes[id].parent);
    return nodes[id].parent;
}