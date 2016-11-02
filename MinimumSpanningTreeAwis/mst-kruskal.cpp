
#include<iostream>
#include<algorithm>
#include<queue>
#include<vector>
#include<set>
#include<functional>

#include "graph.h"
#include "disjointset.h"

using namespace std;
// The main function that sort the given string arr[] in
// alphabatical order
Edge** countSort(Edge** arr, int size)
{
    Edge** output;
    output = new Edge*[size];
 
    int* count = new int[10000];
    int i;
 
    for(i = 0; i < size; ++i)
    {
        ++count[arr[i]->weight];
    }
        
    for (i = 1; i <= 10000; ++i)
    {
        count[i] += count[i-1];
    }
        
    for (i = 0; i<size; ++i)
    {
        output[count[arr[i]->weight]-1] = arr[i];
        count[arr[i]->weight]--;
    }
 
    return output;
}


long kruskal(Graph* G){
    //Edge** mst = new Edge*[G->numVertices]; 

    Edge** edgeList = G->edgeList;
    
    //sort(edgeList, edgeList + G->numEdges, edgeSort);
    edgeList = countSort(edgeList, G->numEdges);

    DisjointSet* ds = new DisjointSet(G->vertexList, G->numVertices);


    Edge* e;
    Vertex* n1;
    Vertex* n2;
    Random randGenerator(0);
    unsigned int hash = 0;

    for(int i = 0; i<G->numEdges; i++)
    {
        e = edgeList[i];
        n1 = ds->find(e->v1->id);
        n2 = ds->find(e->v2->id);
        if(!(n1 == n2))
        {
            ds->setUnion(n1, n2);
            //mst[j++] = e;
            hash += randGenerator.hashRand(e->weight);
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

    cout <<  kruskal(G) << endl;
    //cout <<  kruskalArray(G) << endl;
}




