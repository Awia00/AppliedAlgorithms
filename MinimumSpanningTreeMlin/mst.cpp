
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

            if (nodes[n1].rank < nodes[n2].rank) {
                // n2 has largest rank:
                nodes[n1].parent = n2;
            } else if (nodes[n2].rank < nodes[n1].rank) {
                // n1 has largest rank:
                nodes[n2].parent = n1;
            } else {
                nodes[n1].parent = n2;
                nodes[n2].rank++;
            }
        }

        DisjointSet(long numVertices){
            nodes = new UF[numVertices];

            for(long i = 0; i < numVertices; i++) {
                nodes[i].parent=i;
            }
        }

        bool sameSet(long v1, long v2)
        {
            long n1, n2;

            n1 = find(v1);
            n2 = find(v2);

            //return true if v1 and v2 are in the same set 
            //(i.e. have same highest node) false otherwise
            return n1 == n2;
        }

        long find(long id) {
            if (id == nodes[id].parent) return id;
            nodes[id].parent = find(nodes[id].parent);
            return nodes[id].parent;
        }

    private:
        struct UF 
        {
            long parent;
            int rank;
        };
        UF *nodes;
};

long boruvkaMST(const Graph *graph, const long numVertices)
{
    // Get data of given graph
    const long numEdges = graph->numEdges;
    Edge **edgeList = graph->edgeList;
 
    DisjointSet *set = new DisjointSet(numVertices);
 
    // An array to store index of the cheapest edge of
    // subset.  The stored index for indexing array 'edge[]'
    int *cheapest = new int[numVertices];
 
    // Create V subsets with single elements
    //#pragma omp parallel for shared(cheapest) default(none)
    for (long v = 0; v < numVertices; ++v)
    {
        cheapest[v] = -1;
    }
 
    // Initially there are V different trees.
    // Finally there will be one tree that will be MST
    long numTrees = numVertices;
    unsigned int hash = 0;
    Random randGenerator(0);

    // Keep combining components (or sets) until all
    // compnentes are not combined into single MST.
    while (numTrees > 1)
    {
        // Traverse through all edges and update
        // cheapest of every component
        //#pragma omp parallel for shared(set, edgeList, cheapest) default(none)
        for (long i=0; i<numEdges; i++)
        {
            Edge *e = edgeList[i];
            // Find components (or sets) of two corners
            // of current edge
            long set1 = set->find(e->v1->id);
            long set2 = set->find(e->v2->id);
 
            // If two corners of current edge belong to
            // same set, ignore current edge
            if (set1 == set2)
                continue;
 
            // Else check if current edge is closer to previous
            // cheapest edges of set1 and set2
            if (cheapest[set1] == -1 || edgeList[cheapest[set1]]->weight > e->weight)
                cheapest[set1] = i;
 
            if (cheapest[set2] == -1 || edgeList[cheapest[set2]]->weight > e->weight)
                cheapest[set2] = i;
        }
 
        // Consider the above picked cheapest edges and add them
        // to MST
        for (int i=0; i<numVertices; i++)
        {
            // Check if cheapest for current set exists
            if (cheapest[i] != -1)
            {
                Edge *cheap = edgeList[cheapest[i]];
                long set1 = set->find(cheap->v1->id);
                long set2 = set->find(cheap->v2->id);
 
                if (set1 == set2)
                    continue;

                hash += randGenerator.hashRand(cheap->weight); 
#ifndef CODEJUDGE
                printf("%ld -> %ld\n", cheap->v1->id, cheap->v2->id);
#endif
                // Do a union of set1 and set2 and decrease number
                // of trees
                set->sameSet(set1, set2);
                numTrees--;
            }
        }
    }
 
    return hash;
}

long kruskal(const Graph* G, const long numVertices){
    Edge** edgeList = G->edgeList;
    const long numEdges = G->numEdges;
    DisjointSet *set;
    
    #pragma omp task shared(edgeList) default(none)
    sort(edgeList, edgeList+numEdges, edgeSort);

    #pragma omp task shared(set) default(none)
    set = new DisjointSet(numVertices);

    unsigned int hash = 0;
    Random randGenerator(0);
    int edgeNumber = 0;
    Edge *e; Vertex *v1, *v2; 
    #pragma omp taskwait

    for(int i = 0; i < numEdges && edgeNumber < numVertices-1; i++) {
        e = edgeList[i];
        v1 = e->v1, v2 = e->v2;

        if (!set->sameSet(v1->id, v2->id)) {
#ifndef CODEJUDGE
            printf("%ld -> %ld\n", v1->id, v2->id);
#endif

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

    cout <<  boruvkaMST(G, numVertices) << endl;

#ifndef CODEJUDGE
    #pragma omp parallel
    #pragma omp single nowait
    cout <<  kruskal(G, numVertices) << endl;
#endif
}

