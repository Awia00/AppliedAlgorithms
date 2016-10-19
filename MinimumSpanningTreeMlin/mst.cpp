
#include<iostream>
#include<algorithm>

#include "graph.h"

using namespace std;
bool edgeSort(Edge *i, Edge *j) {return i->weight < j->weight;}

class DisjointSet{
    public:
        void setUnion(long v1, long v2){
            long n1 = find(v1);
            long n2 = find(v2);

            if (n1 == n2) return;

            int rank1 = ranks[n1], rank2 = ranks[n2];

            if (rank1 < rank2) {
                // n2 has largest rank:
                ids[n1] = n2;
            } else if (rank2 < rank1) {
                // n1 has largest rank:
                ids[n2] = n1;
            } else {
                ids[n1] = n2;
                ranks[n2]++;
            }
        }

        DisjointSet(long numVertices){
            ids = new long[numVertices];
            ranks = new int[numVertices];

            for(long i = 0; i < numVertices; i++) {
                ids[i]=i;
            }
        }

        bool sameSet(long v1, long v2)
        {
            //return true if v1 and v2 are in the same set 
            //(i.e. have same highest node) false otherwise
            return find(v1) == find(v2);
        }

    private:
        long *ids;
        int *ranks;

        long find(long id) {
            if (id == ids[id]) return id;
            ids[id] = find(ids[id]);
            return ids[id];
        }
};

long kruskal(Graph* G, long numVertices){
    unsigned int hash = 0;
    Random randGenerator(0); 

    Edge** edgeList = G->edgeList;
    long numEdges = G->numEdges;

    sort(edgeList, edgeList+numEdges, edgeSort);
    
    DisjointSet *set = new DisjointSet(numVertices);

    int edgeNumber = 0;
    Edge *e; Vertex *v1, *v2;
    for(int i = 0; i < numEdges && edgeNumber < numVertices-1; i++) {
        e = edgeList[i];
        v1 = e->v1, v2 = e->v2;

        if (!set->sameSet(v1->id, v2->id)) {
            hash += randGenerator.hashRand(e->weight);
            set->setUnion(v1->id, v2->id);
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

