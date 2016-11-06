#ifndef GRAPHHEADER

#define GRAPHHEADER

//header for graph class
#include <stddef.h>
#include <iostream>
#include <string>

struct HeapEdge {
    long weight;
    long vertex;
};

class Vertex{
    public:
        long id; 
        HeapEdge* vertexEdgeList;
        long totalNumEdges;
        long currentNumEdges;
        Vertex(long inId, long inNumEdges){
            id = inId;
            totalNumEdges = inNumEdges;
            vertexEdgeList = new HeapEdge[totalNumEdges];
            currentNumEdges = 0;
        }
        void addEdge(long vertex, long weight);
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
};

#endif