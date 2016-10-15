
#include<iostream>
#include<algorithm>

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

        void setUnion(long v1, long v2){
            Node *n1 = find(v1);
            Node *n2 = find(v2);

            if (n1->rank < n2->rank) {
                // n2 has largest rank:
                n1->parent = n2;
            } else if (n2->rank < n1->rank) {
                // n1 has largest rank:
                n2->parent = n1;
            } else {
                n1->parent = n2;
                n2->rank++;
            }
        }

        Node* find(long id) {
            //return the address of the highest node in the tree
            Node *n = nodes[id];

            while (n->parent != n) {
                n = n->parent;
            }

            return n;
        }

        DisjointSet(long numVertices){
            nodes = new Node*[numVertices];

            for(int i = 0; i < numVertices; i++) {
                nodes[i] = new Node();
            }
        }

        bool sameSet(long v1, long v2)
        {
            //return true if v1 and v2 are in the same set 
            //(i.e. have same highest node) false otherwise
            return find(v1) == find(v2);
        }

    private:
        Node **nodes;
};

bool edgeSort(Edge *i, Edge *j) {return i->weight < j->weight;}

long kruskal(Graph* G){
    Edge** mst = new Edge*[G->numVertices]; 

    //set up edge list
    Edge** edgeList = new Edge*[G->numEdges];

    for(int i = 0; i < G->numEdges; i++) {
        edgeList[i] = G->edgeList[i];
    }

    std::sort(edgeList, edgeList+G->numEdges, edgeSort);

    DisjointSet *set = new DisjointSet(G->numVertices);

    int edgeNumber = 0;
    for(int i = 0; i < G->numEdges; i++) {
        Edge *e = edgeList[i];

        if (!set->sameSet(e->v1->id, e->v2->id)) {
            mst[edgeNumber++] = e;
            set->setUnion(e->v1->id, e->v2->id);
        }
    }

    return G->mstToInt(mst, edgeNumber);
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
