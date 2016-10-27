#ifndef DaryHeapHeader

#define DaryHeapHeader
#include "graph.h"

struct Node{
    public:
        long id;
        long weight;
        Edge* edge;
        Node()
        {
        }
        Node(long id, long weight, Edge* edge) : id(id), weight(weight), edge(edge)
        {

        }
};
class DaryHeap{
    public:
        Node* heap;
        long* vertexToHeapIndex;

        int d;
        int last = 0;
        
        DaryHeap(long numVertices, int dary) {
            heap = new Node[numVertices];
            vertexToHeapIndex = new long[numVertices];
            for(int i = 0; i<numVertices; i++)
            {
                vertexToHeapIndex[i] = -1;
            }
            d = dary;
        }

        void insert(Vertex* to, Edge* by);

        Edge* pickTop();
    
    private:
        long getParent(long index);
        long getFirstChild(long index);
        long getSecondChild(long index);
        Node* getChildArray(long index);
        void swap(long from, long to);
        void moveUp(long index, long w);
};

#endif