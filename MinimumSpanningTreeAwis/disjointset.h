#ifndef DisjointSetHeader

#define DisjointSetHeader
#include "graph.h"
class DisjointSet{
    public:
        Vertex** nodes;
        DisjointSet(Vertex** v, long numVertices) {
            nodes = v;
        }

        Vertex* find(long id);
        void setUnion(long v1, long v2);
        void setUnion(Vertex* parent1, Vertex* parent2);
        bool sameSet(long v1, long v2);
};
#endif