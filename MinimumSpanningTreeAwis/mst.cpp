
#include<iostream>
#include <algorithm>

#include "graph.h"


long prim(Graph* G){
    Edge** mst = new Edge*[G->numVertices];
    long mstsize = 0;

    return G->mstToInt(mst,mstsize); 
}

class DisjointSet{
    public:
        class Node{
            //Node is potentially a helpful class.  Can be changed or deleted without harm
            public:
                Node* parent;
                int rank;
                Node(){
                    rank = 0;
                    parent = this;
                }
        };

        Node** nodes;
        DisjointSet(Vertex** v, long numVertices) {
            nodes = new Node*[numVertices];
            for(long i = 0; i<numVertices; i++)
            {
                long id = v[i]->id;
                nodes[id] = new Node();
            }
        }

        void setUnion(long v1, long v2){
            Node* parent1 = find(v1);
            Node* parent2 = find(v2);
            if(parent1->rank > parent2->rank)
            {
                parent1->parent = parent2;
            }
            else if(parent1->rank < parent2->rank)
            {
                parent2->parent = parent1;
            }
            else
            {
                parent1->parent = parent2;
                parent2->rank++;
            }
        }

        Node* find(long id) {
            //return NULL;
            Node* first = nodes[id];
            Node* prev = first;
            while(prev->parent != prev)
            {
                prev = prev->parent;
            }
            Node* leader = prev;
            prev = first;
            // path compression
            while(prev->parent != prev)
            {
                Node* newParent = prev->parent;
                prev->parent = leader;
                prev = newParent;
            }
            return leader;
        }

        bool sameSet(long v1, long v2)
        {
            return find(v1) == find(v2);
        }
};

long kruskal(Graph* G){
    Edge** mst = new Edge*[G->numVertices]; 

    Edge** edgeList = G->edgeList;
    
    std::sort(edgeList, edgeList + G->numEdges, Edge::compare);

    DisjointSet* ds = new DisjointSet(G->vertexList, G->numVertices);

    long j = 0;
    for(long i = 0; i<G->numEdges; i++)
    {
        Edge* e = edgeList[i];
        if(!ds->sameSet(e->v1->id, e->v2->id))
        {
            ds->setUnion(e->v1->id, e->v2->id);
            mst[j++] = e;
        }
    }
    return G->mstToInt(mst, j);
}



int main(int argc, char* argv[]){
    long numVertices, numEdges;
    unsigned int seed;
    std::string infile = "";
    Graph* G;

    if(argc == 4)
    {
        //file name, numVertices, numEdges
        numVertices = std::atol(argv[2]);
        numEdges = std::atol(argv[3]);
        G = new Graph(numVertices,numEdges);
        G->graphFromFile(argv[1], seed);
    } 
    else if(argc == 3)
    {
        //random seed, numVertices
        seed = std::atol(argv[1]);
        numVertices = std::atol(argv[2]);
        numEdges = numVertices*(numVertices - 1)/2; 
        G = new Graph(numVertices,numEdges);
        G->generateRandomWeights(seed); 
    }
    else if(argc == 5)
    {
        //random seed, numX, numY, skip probability
        seed = std::atol(argv[1]);
        long numX = std::atol(argv[2]);
        long numY = std::atol(argv[3]);
        int skipProb = atoi(argv[4]);
        srand(seed);
        numVertices = numX*numY;
        numEdges = numX*(numY - 1) + numY*(numX - 1);
        G = new Graph(numVertices,numEdges);
        G->generateGrid(numX,numY,skipProb,seed);
    }
    else
    {
        std::cout << "Error: " << argc - 1 << "arguments; should be 2, 3, or 4\n";
        return 0;
    }

    std::cout <<  kruskal(G) << std::endl;
    //std::cout <<  prim(G) << std::endl;
}
