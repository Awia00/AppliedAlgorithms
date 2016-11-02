
#include<iostream>
#include<algorithm>
#include<queue>
#include<vector>
#include<set>
#include<functional>

#include "graph.h"
#include "daryheap.h"
#include "disjointset.h"

using namespace std;

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

long primHeap(Graph* G, int d){
    DaryHeap* heap = new DaryHeap(G->numVertices, d);
    
    bool* hasBeenTaken = new bool[G->numVertices];

    long toPick = 0;
    long howManyTimes = 0;
    Random randGenerator(0);
    unsigned int hash = 0;
    while(toPick < G->numVertices)
    {
        howManyTimes++;
        while(toPick < G->numVertices && hasBeenTaken[toPick]) 
             toPick++;
        if (toPick >= G->numVertices) 
            break;

        Vertex* v = G->vertexList[toPick];
        hasBeenTaken[v->id] = true;
        Edge** edges = v->vertexEdgeList;
        for(int i = 0; i < v->currentNumEdges; i++)
        {
            Vertex* to = v->id != edges[i]->v1->id ? edges[i]->v1 : edges[i]->v2;
            heap->insert(to, edges[i]);
        }
        while(heap->last != 0)
        {
            Edge* e = heap->pickTop();

            if(hasBeenTaken[e->v1->id] && hasBeenTaken[e->v2->id])
                continue;
            
            hash += randGenerator.hashRand(e->weight);
        
            if(!hasBeenTaken[e->v1->id])
            {
                hasBeenTaken[e->v1->id] = true;
                for(int i = 0; i < e->v1->currentNumEdges; i++)
                {
                    Edge* newEdge = e->v1->vertexEdgeList[i];

                    if(!(hasBeenTaken[newEdge->v1->id] && hasBeenTaken[newEdge->v2->id]))
                    {
                        Vertex* to = (e->v1->id != newEdge->v1->id ? newEdge->v1 : newEdge->v2);
                        heap->insert(to, newEdge);
                    }
                }
            }
            else if(!hasBeenTaken[e->v2->id])
            {
                hasBeenTaken[e->v2->id] = true;
                for(int i = 0; i < e->v2->currentNumEdges; i++)
                {
                    Edge* newEdge = e->v2->vertexEdgeList[i];
                    if(!(hasBeenTaken[newEdge->v1->id] && hasBeenTaken[newEdge->v2->id]))
                    {
                        Vertex* to = (e->v2->id != newEdge->v2->id ? newEdge->v2 : newEdge->v1);
                        heap->insert(to, newEdge);
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
        cout << kruskal(G) << endl;
    } 
    else if(argc == 3)
    {
        //random seed, numVertices
        seed = atol(argv[1]);
        numVertices = atol(argv[2]);
        numEdges = numVertices*(numVertices - 1)/2; 
        G = new Graph(numVertices,numEdges);
        G->generateRandomWeights(seed); 
        cout << primHeap(G, 8) << endl;
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

        if(skipProb > 20)
            cout << kruskal(G) << endl;
        else
            cout << primHeap(G, 8) << endl;
    }
    else
    {
        cout << "Error: " << argc - 1 << "arguments; should be 2, 3, or 4\n";
        return 0;
    }

    // for(int i = 0; i < G->numEdges; i++)
    // {
    //     cout << G->edgeList[i]->v1->id << ", " << G->edgeList[i]->v2->id << ": " << G->edgeList[i]->weight << endl;
    // }
    // cout << endl;
}




