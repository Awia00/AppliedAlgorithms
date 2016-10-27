
#include<iostream>
#include<algorithm>
#include<queue>
#include<set>
#include<functional>
#include "graph.h"
#include "minheap.h"

using namespace std;
//bool edgeRevSort(Edge *i, Edge *j) {return i->weight > j->weight;}

long prim(Graph* G){
    unsigned int hash = 0;
    Random randGenerator(0);
    long mstsize = 0;

    MinHeap fringe = MinHeap(G->numVertices);

    bool *nodeSet = new bool[G->numVertices];

    Vertex* v = G->vertexList[G->numVertices - 1];
    long vId = v->id;
    nodeSet[vId] = true; 

    for(int i = 0; i < v->currentNumEdges; i++) {
        Edge *e = v->vertexEdgeList[i];

        if (e->v1->id == vId) {
            if (!nodeSet[e->v2->id]) {
                int curWeight = fringe.weightOfVertex(e->v2->id);

                if (curWeight == -1) {
                    fringe.insert(e->v2->id, e->weight);
                } else if (curWeight > e->weight) {
                    fringe.decreaseKey(e->v2->id, e->weight);
                }
            }
        } else {
            if (!nodeSet[e->v1->id]) {
                int curWeight = fringe.weightOfVertex(e->v1->id);

                if (curWeight == -1) {
                    fringe.insert(e->v1->id, e->weight);
                } else if (curWeight > e->weight) {
                    fringe.decreaseKey(e->v1->id, e->weight);
                }
            }
        }
    }

    while(mstsize < G->numVertices - 1 && fringe.any()) {
        HeapEdge heapEdge = fringe.extractMin();
        
        if (nodeSet[heapEdge.vertex]) { cout << "Should never happen" << endl; continue; }

        nodeSet[heapEdge.vertex] = true;

        //cout << "Chose: " << heapEdge.weight << endl;

        hash += randGenerator.hashRand(heapEdge.weight);
        mstsize++;

        v = G->vertexList[heapEdge.vertex];

        for(int i = 0; i < v->currentNumEdges; i++) {
            Edge *e = v->vertexEdgeList[i];

            long v1 = e->v1->id, v2 = e->v2->id;

            if (v1 == heapEdge.vertex) {
                if (!nodeSet[v2]) {
                    int curWeight = fringe.weightOfVertex(v2);

                    if (curWeight == -1) {
                        fringe.insert(v2, e->weight);
                    } else if (curWeight > e->weight) {
                        fringe.decreaseKey(v2, e->weight);
                    }
                }
            } else {
                if (!nodeSet[v1]) {
                    int curWeight = fringe.weightOfVertex(v1);

                    if (curWeight == -1) {
                        fringe.insert(v1, e->weight);
                    } else if (curWeight > e->weight) {
                        fringe.decreaseKey(v1, e->weight);
                    }
                }
            }
        }
    }

    return hash;
}

int main(int argc, char* argv[]){
    long numVertices, numEdges;
    unsigned int seed = 0;
    string infile = "";
    Graph* G;

    if(argc == 4)
    {
        //file name, numVertices, numEdges
        numVertices = atol(argv[2]);
        numEdges = atol(argv[3]);
        G = new Graph(numVertices,numEdges);
        G->graphFromFile(argv[1], seed);
    } 
    else if(argc == 3)
    {
        //random seed, numVertices
        seed = atol(argv[1]);
        numVertices = atol(argv[2]);
        numEdges = numVertices*(numVertices - 1)/2; 
        G = new Graph(numVertices,numEdges);
        G->generateRandomWeights(seed); 
    }
    else if(argc == 5)
    {
        //random seed, numX, numY, skip probability
        seed = atol(argv[1]);
        long numX = atol(argv[2]);
        long numY = atol(argv[3]);
        int skipProb = atoi(argv[4]);
        srand(seed);
        numVertices = numX*numY;
        numEdges = numX*(numY - 1) + numY*(numX - 1);
        G = new Graph(numVertices,numEdges);
        G->generateGrid(numX,numY,skipProb,seed);
    }
    else
    {
        cout << "Error: " << argc - 1 << "arguments; should be 2, 3, or 4\n";
        return 0;
    }
    cout <<  prim(G) << endl;
}
