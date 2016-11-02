#include<iostream>
#include<algorithm>
#include<queue>
#include<set>
#include<functional>
#include<stdexcept>
#include "graph.h"
#include "minheap.h"

using namespace std;

long prim(Graph* G, const long numVertices){
    unsigned int hash = 0;
    Random randGenerator(0);

    MinHeap fringe = MinHeap(numVertices);

    bool *nodeSet = new bool[numVertices];

    long scan = 0;

    while (scan < numVertices) {
        while (scan < numVertices && nodeSet[scan]) scan++;

        if (scan >= numVertices) break;

        Vertex* v = G->vertexList[scan];
        nodeSet[scan] = true;

        for(int i = 0; i < v->currentNumEdges; i++) {
            HeapEdge e = v->vertexEdgeList[i];

            if (!nodeSet[e.vertex]) {
                int curWeight = fringe.weightOfVertex(e.vertex);

                if (curWeight == -1) {
                    fringe.insert(e.vertex, e.weight);
                } else if (curWeight > e.weight) {
                    fringe.decreaseKey(e.vertex, e.weight);
                }
            }
            
        }

        while(fringe.size != 0) {
            HeapEdge heapEdge = fringe.extractMin();
            
            nodeSet[heapEdge.vertex] = true;

            hash += randGenerator.hashRand(heapEdge.weight);

            v = G->vertexList[heapEdge.vertex];

            for(int i = 0; i < v->currentNumEdges; i++) {
                HeapEdge e = v->vertexEdgeList[i];

                if (!nodeSet[e.vertex]) {
                    int curWeight = fringe.weightOfVertex(e.vertex);

                    if (curWeight == -1) {
                        fringe.insert(e.vertex, e.weight);
                    } else if (curWeight > e.weight) {
                        fringe.decreaseKey(e.vertex, e.weight);
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
    cout <<  prim(G, numVertices) << endl;
}
