#include <fstream>
#include <iostream>
#include "graph.h"

using namespace std;
 
unsigned int Random::rand(){
    //should make sufficiently random numbers
    //only low-order bits have good randomness; works for us because we use mod 1000
    for(int i = 0; i < 8; i++)
        nextIndex = (nextIndex + 1)*( (nextIndex >> 1)^b);
    return nextIndex>>32;
}
 
unsigned int Random::hashRand(long inIndex){
    //should make sufficiently random numbers
    //only low-order bits have good randomness; works for us because we use mod 1000
    for(int i = 0; i < 8; i++)
        inIndex = (inIndex + 1)*( (inIndex >> 1)^b);
    return inIndex>>32;
}
 
void Graph::generateRandomWeights(unsigned long seed){
    if(numVertices*(numVertices - 1)/2 != numEdges)
    {
        std::cout << "In GenerateRandomWeights should have " << numVertices*(numVertices - 1)/2 << " edges but instead have " << numEdges << std::endl;
    }
    edgeList = new Edge[numEdges];
    
    long edgeweight; 
    long counter = 0;
    Random randGenerator(seed);
    for(long v1 = 0; v1 < numVertices; v1++)
    {
        for(long v2 = v1+1; v2 < numVertices; v2++)
        {
            edgeweight = randGenerator.rand()%10000;
            Edge e;
            e.weight = edgeweight;
            e.v1 = v1;
            e.v2 = v2;
            edgeList[counter++] = e;
        }
    }
}
 
void Graph::graphFromFile(std::string infile, unsigned long seed){
    std::ifstream gFile; 
    gFile.open(infile.c_str());
    if(!gFile.is_open()){
        std::cout << "Error opening file " << infile << std::endl;
        return;
    }
 
    edgeList = new Edge[numEdges];
    long counter = 0;
    std::string nextLine;
    long v1,v2;
    Random randGenerator(seed);
    while(!gFile.eof())
    {
        std::getline(gFile, nextLine);
         
        //if the line contains # ignore it
        if(nextLine.find('#') == std::string::npos && nextLine != "")
        {
            //first and second vertices are split by a tab
            std::size_t split = nextLine.find('\t');
            
            v1 = atol(nextLine.substr(0,split).c_str());
            v2 = atol(nextLine.substr(split,nextLine.size()-split).c_str());
                
            long edgeWeight = randGenerator.rand()%(10000);
            Edge e;
            e.weight = edgeWeight;
            e.v1 = v1;
            e.v2 = v2;
            edgeList[counter++] = e;
        }
    }
    
    // Weird bug? If this isn't 'printed' then the program gets an segmentation fault. 
    cout << '\0';
    
    gFile.close();
}
 
void Graph::generateGrid(long numX, long numY, int skipProb, unsigned long seed){
    edgeList = new Edge[numEdges];
    long edgeweight; 
    long counter = 0;
    Random randGenerator(seed);
 
    //horizontal edges
    for(long x = 0; x < numX - 1; x++)
    {
        for(long y = 0; y < numY; y++) 
        { 
            if(randGenerator.rand()%skipProb == 0)
            {
                edgeweight = randGenerator.rand()%(10000);

                Edge e;
                e.weight = edgeweight;
                e.v1 = x + y*numX;
                e.v2 = x + y*numX + 1;

                edgeList[counter++] = e;
            }
        }
    }
 
    //vertical edges
    for(long x = 0; x < numX; x++)
    {
        for(long y = 0; y < numY-1; y++) 
        { 
            if(randGenerator.rand()%skipProb == 0)
            {
                edgeweight = randGenerator.rand()%(10000);
                Edge e;
                e.weight = edgeweight;
                e.v1 = x + y*numX;
                e.v2 = x + (y+1)*numX;

                edgeList[counter++] = e;
            }
        }
    }
    numEdges = counter;
}