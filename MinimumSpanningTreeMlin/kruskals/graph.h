#ifndef GraphHeader
 
#define GraphHeader
 
//header for graph class
#include <stddef.h>
#include <string>

struct Edge {
    long weight, v1, v2;
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
        Edge* edgeList; 
        Graph(long inNumVertices, long inNumEdges){
            numVertices = inNumVertices;
            numEdges = inNumEdges;
            edgeList = NULL;
        }
        void generateRandomWeights(unsigned long seed);
        void generateGrid(long numX, long numY, int skipProb,unsigned long seed);
        void graphFromFile(std::string infile, unsigned long seed);
};
 
#endif