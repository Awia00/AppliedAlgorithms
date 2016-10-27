#ifndef DisjointSetArrayHeader

#define DisjointSetArrayHeader
#include "graph.h"

class DisjointSetArray{
    public:
        int* parents;
        int* ranks;

        DisjointSetArray(Vertex** v, int numVertices)
        {
            parents = new int[numVertices];
            ranks = new int[numVertices];
            for(int i = 0; i<numVertices; i++)
            {
                parents[v[i]->id] = v[i]->id;
                ranks[v[i]->id] = 10000;
            }
        }
        
        void setUnion(int v1, int v2);

        void setUnionParents(int parent1, int parent2);

        int find(int id);

        bool sameSet(int v1, int v2)
        {
            return find(v1) == find(v2);
        }
};


#endif