
#include<iostream>
#include<algorithm>
#include<queue>
#include<vector>
#include<set>
#include<functional>

#include "graph.h"
#include "daryheap.h"

using namespace std;

long primPrio(Graph* G){
    Edge** mst = new Edge*[G->numVertices];
    priority_queue<Edge*, vector<Edge*>, function<bool(Edge*, Edge*)>> pq(Edge::compare2);
    
    bool* hasBeenTaken = new bool[G->numVertices];

    long j = 0;
    Vertex* v = G->vertexList[j];
    hasBeenTaken[v->id] = true;
    for(int i = 0; i < v->currentNumEdges; i++)
    {
        pq.push(v->vertexEdgeList[i]);
    }
    
    while(!pq.empty())
    {
        Edge* e = pq.top();
        pq.pop();
        
        if(hasBeenTaken[e->v1->id] && hasBeenTaken[e->v2->id])
            continue;
        
        mst[j++] = e;
    
        if(!hasBeenTaken[e->v1->id])
        {
            for(int i = 0; i < e->v1->currentNumEdges; i++)
            {
                Edge* newEdge = e->v1->vertexEdgeList[i];

                if(!(hasBeenTaken[newEdge->v1->id] && hasBeenTaken[newEdge->v2->id]))
                    pq.push(newEdge);
            }
            hasBeenTaken[e->v1->id] = true;
        }
        if(!hasBeenTaken[e->v2->id])
        {
            for(int i = 0; i < e->v2->currentNumEdges; i++)
            {
                Edge* newEdge = e->v2->vertexEdgeList[i];
                if(!(hasBeenTaken[newEdge->v1->id] && hasBeenTaken[newEdge->v2->id]))
                    pq.push(newEdge);
            }    
            hasBeenTaken[e->v2->id] = true;
        }
        
        

    }
    return G->mstToInt(mst, j); 
}

long primHeap(Graph* G){
    Edge** mst = new Edge*[G->numVertices];
    DaryHeap* heap = new DaryHeap(G->numVertices, 2);
    
    bool* hasBeenTaken = new bool[G->numVertices];

    long j = 0;
    Vertex* v = G->vertexList[j];
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
        
        mst[j++] = e;
    
        if(!hasBeenTaken[e->v1->id])
        {
            for(int i = 0; i < e->v1->currentNumEdges; i++)
            {
                Edge* newEdge = e->v1->vertexEdgeList[i];

                if(!(hasBeenTaken[newEdge->v1->id] && hasBeenTaken[newEdge->v2->id]))
                {
                    Vertex* to = (e->v1->id != newEdge->v1->id ? newEdge->v1 : newEdge->v2);
                    heap->insert(to, newEdge);
                }
                    
            }
            hasBeenTaken[e->v1->id] = true;
        }
        if(!hasBeenTaken[e->v2->id])
        {
            for(int i = 0; i < e->v2->currentNumEdges; i++)
            {
                Edge* newEdge = e->v2->vertexEdgeList[i];
                if(!(hasBeenTaken[newEdge->v1->id] && hasBeenTaken[newEdge->v2->id]))
                {
                    Vertex* to = (e->v2->id != newEdge->v2->id ? newEdge->v2 : newEdge->v1);
                    heap->insert(to, newEdge);
                }
            }    
            hasBeenTaken[e->v2->id] = true;
        }
        
        

    }
    return G->mstToInt(mst, j); 
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

    cout <<  primPrio(G) << endl;
    cout <<  primHeap(G) << endl;
}




