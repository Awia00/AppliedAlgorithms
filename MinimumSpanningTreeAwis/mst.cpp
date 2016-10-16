
#include<iostream>
#include<algorithm>
#include<queue>
#include<vector>

#include "graph.h"

class PrimVertex{
    public:
        Vertex* v;
        long cost;
        PrimVertex(Vertex* vertex)
        {
            v = vertex;
            cost = 10000;
        }
        PrimVertex(Vertex* vertex, long aCost)
        {
            v = vertex;
            cost = aCost;
        }
        static bool compare(PrimVertex* a, PrimVertex* b)
        {
            return a->cost < b->cost;
        }
};

long prim(Graph* G){
    Edge** mst = new Edge*[G->numVertices];
    std::priority_queue<PrimVertex*, std::vector<PrimVertex*>, std::function<bool(PrimVertex*, PrimVertex*)>> pq(PrimVertex::compare);
    
    bool* hasBeenTaken = new bool[G->numVertices];
    Edge** cheapestRoute = new Edge*[G->numVertices];

    long j = 0;
    pq.push(new PrimVertex(G->vertexList[j]));
    bool notFirstRound = false;
    while(!pq.empty())
    {
        PrimVertex* p = pq.top();
        pq.pop();
        long id = p->v->id;
        
        if(!hasBeenTaken[id]) 
        {
            hasBeenTaken[id] = true;
            Edge** edges = p->v->vertexEdgeList;
            for(long i = 0; i < p->v->totalNumEdges; i++)
            {
                Edge* edge = edges[i];
                if (cheapestRoute[edge->v1->id] == NULL || cheapestRoute[edge->v1->id]->weight > edge->weight)
                {
                    cheapestRoute[edge->v1->id] = edge;
                    PrimVertex* pv = new PrimVertex(edge->v1, edge->weight);
                    pq.push(pv);
                }
                if (cheapestRoute[edge->v2->id] == NULL || cheapestRoute[edge->v2->id]->weight > edge->weight)
                {
                    cheapestRoute[edge->v2->id] = edge;
                    PrimVertex* pv = new PrimVertex(edge->v2, edge->weight);
                    pq.push(pv);
                }
            }

            if(notFirstRound)
            {
                mst[j++] = cheapestRoute[id];
            }
            else
                notFirstRound = true;
            
        }
    }
    return G->mstToInt(mst, j); 
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
    std::cout <<  prim(G) << std::endl;
}
