//header for graph class
#include <stddef.h>
#include <iostream>
#include <string>

class Edge;

class Vertex{
    public:
        long id; 
        Edge** vertexEdgeList;
        long totalNumEdges;
        long currentNumEdges;
        Vertex(long inId, long inNumEdges, Edge** inEdgeList = NULL){
            id = inId;
            totalNumEdges = inNumEdges;
            vertexEdgeList =  new Edge*[totalNumEdges];
            currentNumEdges = 0;
        }
        void addEdge(Edge* e);
};

class Edge{
    public:
        Vertex* v1;
        Vertex* v2;
        long weight;
        Edge(long inWeight = 1, Vertex* inV1 = NULL, Vertex* inV2 = NULL){
            v1 = inV1;
            v2 = inV2;
            weight = inWeight;
        }
        void output(){
            std::cout << "(" << v1->id << "," << v2-> id << ") w: " << weight << " ";
        }
};

class Random{
    public:
        const static int b = 0x5f375a86;  //bunch of random bits
        unsigned long nextIndex;
        Random(unsigned long seed){
            nextIndex = seed;
        }
        unsigned int rand();
        unsigned int hashRand(long inIndex);
};


class Graph{
    public:
        long numVertices;
        long numEdges;
        Vertex** vertexList;
        Graph(long inNumVertices = -1, long inNumEdges = -1){
            numVertices = inNumVertices;
            numEdges = inNumEdges;
            vertexList = NULL;
        }
        void generateRandomWeights(unsigned long seed);
        void generateRandomEdgesWeights(unsigned long seed);
        void generateGrid(long numX, long numY, int skipProb,unsigned long seed);
        void graphFromFile(std::string infile, unsigned long seed);
        unsigned int mstToInt(Edge** mst, long mstsize);
};
