#include <iostream>
#include "graph.h"
#include "disjointset.h"
#include "countingsort.h"

using namespace std;
bool edgeSort(Edge *i, Edge *j) {return i->weight < j->weight;}

long kruskal(const Graph* G, const long numVertices){
    Edge *edgeList = G->edgeList;
    const long numEdges = G->numEdges;
    DisjointSet *set = new DisjointSet(numVertices);

    edgeList = countingSort(edgeList, numEdges, 20000);

    unsigned int hash = 0;
    Random randGenerator(0);
    int edgeNumber = 0;
    Edge e; long v1, v2; 

    for(int i = 0; i < numEdges && edgeNumber < numVertices-1; i++) {
        e = edgeList[i];
        v1 = e.v1, v2 = e.v2;

        if (!set->sameSet(v1, v2)) {
            hash += randGenerator.hashRand(e.weight);
            set->setUnion(v1, v2);
            edgeNumber++;
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

    cout <<  kruskal(G, numVertices) << endl;
}

