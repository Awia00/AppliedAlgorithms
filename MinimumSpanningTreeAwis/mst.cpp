
#include<iostream>
#include<algorithm>
#include<queue>
#include<vector>
#include<set>
#include<functional>

#include "graph.h"

using namespace std;
//#ifndef CODEJUDGE
long prim(Graph* G){
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
//#endif
class DisjointSetArray{
    public:
        int* parents;
        int* ranks;

        DisjointSetArray(Vertex** v, int numVertices)
        {
            parents = new int[numVertices];
            ranks = new int[numVertices];
            for(int i = 0; i<numVertices; i++)
            {
                parents[v[i]->id] = v[i]->id;
                ranks[v[i]->id] = 10000;
            }
        }
        
        void setUnion(int v1, int v2){
            int parent1 = find(v1);
            int parent2 = find(v2);
            if(ranks[parent1] > ranks[parent2])
            {
                parents[parent2] = parent1;
            }
            else if(ranks[parent1] < ranks[parent2])
            {
                parents[parent1] = parent2;
            }
            else
            {
                parents[parent1] = parent2;
                ranks[parent2]++;
            }
        }

        void setUnionParents(int parent1, int parent2){
            if(ranks[parent1] > ranks[parent2])
            {
                parents[parent2] = parent1;
            }
            else if(ranks[parent1] < ranks[parent2])
            {
                parents[parent1] = parent2;
            }
            else
            {
                parents[parent1] = parent2;
                ranks[parent2]++;
            }
        }

        int find(int id) {
            int first = parents[id];
            int prev = first;
            while(parents[prev] != prev)
            {
                prev = parents[prev];
            }
            
            if(parents[first] == prev) // if the lenght was 0 or 1
                return prev;
                
            int leader = prev;
            prev = first;
            // path compression
            int newParent;
            while(parents[prev] != prev)
            {
                newParent = parents[prev];
                parents[prev] = leader;
                prev = newParent;
            }
            return leader;
        }

        bool sameSet(int v1, int v2)
        {
            return find(v1) == find(v2);
        }

};

class DisjointSet{
    public:
        Vertex** nodes;
        DisjointSet(Vertex** v, long numVertices) {
            nodes = v;
        }

        void setUnion(long v1, long v2){
            Vertex* parent1 = find(v1);
            Vertex* parent2 = find(v2);
            if(parent1->rank > parent2->rank)
            {
                parent2->parent = parent1;
            }
            else if(parent1->rank < parent2->rank)
            {
                parent1->parent = parent2;
            }
            else
            {
                parent1->parent = parent2;
                parent2->rank++;
            }
        }
        void setUnion(Vertex* parent1, Vertex* parent2){
            if(parent1->rank > parent2->rank)
            {
                parent2->parent = parent1;
            }
            else if(parent1->rank < parent2->rank)
            {
                parent1->parent = parent2;
            }
            else
            {
                parent1->parent = parent2;
                parent2->rank++;
            }
        }

        Vertex* find(long id) {
            Vertex* first = nodes[id];
            Vertex* prev = first;
            while(prev->parent != prev)
            {
                prev = prev->parent->parent;
            }
            
            if(first->parent == prev)
                return prev;
                
            Vertex* leader = prev;
            prev = first;
            // path compression
            Vertex* newParent;
            while(prev->parent != prev)
            {
                newParent = prev->parent;
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
bool edgeSort(Edge *i, Edge *j) {return i->weight < j->weight;}
long kruskal(Graph* G){
    Edge** mst = new Edge*[G->numVertices]; 

    Edge** edgeList = G->edgeList;
    
    sort(edgeList, edgeList + G->numEdges, edgeSort);

    DisjointSet* ds = new DisjointSet(G->vertexList, G->numVertices);

    long j = 0;
    Edge* e;

    Vertex* n1;
    Vertex* n2;
    for(int i = 0; i<G->numEdges; i++)
    {
        e = edgeList[i];
        n1 = ds->find(e->v1->id);
        n2 = ds->find(e->v2->id);
        if(!(n1 == n2))
        {
            ds->setUnion(n1, n2);
            mst[j++] = e;
        }
    }

    return G->mstToInt(mst, j);
}

long kruskalArray(Graph* G){
    Edge** mst = new Edge*[G->numVertices]; 

    Edge** edgeList = G->edgeList;
    
    sort(edgeList, edgeList + G->numEdges, Edge::compare);

    DisjointSetArray* ds = new DisjointSetArray(G->vertexList, G->numVertices);

    long j = 0;
    Edge* e;
    // for(int i = 0; i<G->numEdges; i++)
    // {
    //     e = edgeList[i];
    //     if(!ds->sameSet(e->v1->id, e->v2->id))
    //     {
    //         ds->setUnion(e->v1->id, e->v2->id);
    //         mst[j++] = e;
    //     }
    // }

    int n1;
    int n2;
    for(int i = 0; i<G->numEdges; i++)
    {
        e = edgeList[i];
        n1 = ds->find(e->v1->id);
        n2 = ds->find(e->v2->id);
        if(!(n1 == n2))
        {
            ds->setUnionParents(n1, n2);
            mst[j++] = e;
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

    cout <<  kruskal(G) << endl;
    //cout <<  kruskalArray(G) << endl;
    //cout <<  prim(G) << endl;
}
