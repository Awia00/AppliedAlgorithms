#ifndef DisjointSetHeader

#define DisjointSetHeader
#include "graph.h"

struct Node{
    int rank;
    Node* parent;
    Node()
    {
        rank = 1;
        parent = this;
    }
};

class DisjointSet{
    public:
        Node** nodes;
        DisjointSet(Vertex** v, long numVertices) {
            nodes = new Node*[numVertices];
            for(int i = 0; i<numVertices; i++)
            {
                nodes[i] = new Node();
            }
        }

        Node* find(long id);
        void setUnion(long v1, long v2);
        void setUnion(Node* parent1, Node* parent2);
        bool sameSet(long v1, long v2);
};
#endif