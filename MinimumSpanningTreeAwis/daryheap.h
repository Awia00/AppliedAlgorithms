#ifndef DaryHeapHeader

#define DaryHeapHeader
#include "graph.h"

class DaryHeap{
    public:
        int* weights;
        int* vertexIds; // in the min heap structure
        Edge** edgesTo;
        int d;
        int last = 0;
        
        DaryHeap(long numVertices, int dary) {
            weights = new int[numVertices];
            for(int i = 0; i<numVertices; i++)
            {
                weights[i] = 10000;
            }
            vertexIds = new int[numVertices];
            edgesTo = new Edge*[numVertices];
            d = dary;
        }

        void insert(Vertex* to, Edge* by);

        Edge* pickTop();
    
    private:
        long getParent(long index);
        long getFirstChild(long index);
        long getSecondChild(long index);
        int* getChildArray(long index);
        void swap(long from, long to);
};

#endif