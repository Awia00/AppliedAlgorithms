#include <fstream>
#include "graph.h"

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

void Vertex::addEdge(long vertex, long weight) {
    vertexEdgeList[currentNumEdges].vertex = vertex;
    vertexEdgeList[currentNumEdges++].weight = weight;  
    if(currentNumEdges > totalNumEdges)
    {
        std::cout << "Vertex " << id << " has had more than " << totalNumEdges << " edges assigned." << std::endl;
    }
}

void Graph::generateRandomWeights(unsigned long seed){
    if(numVertices*(numVertices - 1)/2 != numEdges)
    {
        std::cout << "In GenerateRandomWeights should have " << numVertices*(numVertices - 1)/2 << " edges but instead have " << numEdges << std::endl;
    }
    vertexList = new Vertex*[numVertices];
    for(long v = 0; v < numVertices; v++)
    {
        vertexList[v] = new Vertex(v,numVertices - 1);
    }
    long edgeweight; 
    Random randGenerator(seed);
    for(long v1 = 0; v1 < numVertices; v1++)
    {
        for(long v2 = v1+1; v2 < numVertices; v2++)
        {
            edgeweight = randGenerator.rand()%10000;
            vertexList[v1]->addEdge(v2, edgeweight);
            vertexList[v2]->addEdge(v1, edgeweight);
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

    vertexList = new Vertex*[numVertices];
    for(long v = 0; v < numVertices; v++)
    {
        vertexList[v] = new Vertex(v,numVertices - 1);
    }

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
            if(split == std::string::npos)
            {
                std::cout << "File input error: non-comment line with no tab:\n" << nextLine << std::endl;
            }
            else
            {
                v1 = atol(nextLine.substr(0,split).c_str());
                v2 = atol(nextLine.substr(split,nextLine.size()-split).c_str());
                if(v1 > numVertices || v2 > numVertices || v1 < 0 || v2 < 0)
                {
                    std::cout << "File input error: \nnumVertices: " << numVertices << " v1: " << v1 << " v2: " << v2 << "\nFile input line: " << nextLine << std::endl;
                }
                else
                {
                    long edgeWeight = randGenerator.rand()%(10000);
                    vertexList[v1]->addEdge(v2, edgeWeight);
                    vertexList[v2]->addEdge(v1, edgeWeight);
                }
            }
        }
    }
    //std::cout << "Done reading graph from file" << std::endl;
    gFile.close();
}

void Graph::generateGrid(long numX, long numY, int skipProb, unsigned long seed){
    vertexList = new Vertex*[numVertices];
    for(long v = 0; v < numVertices; v++)
    {
        //all vertices have degree 4
        vertexList[v] = new Vertex(v,4);
    }
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
                long v1 = x + y*numX, v2 = x + y*numX + 1;
                vertexList[v1]->addEdge(v2, edgeweight);
                vertexList[v2]->addEdge(v1, edgeweight);
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
                long v1 = x + y*numX, v2 = x + (y+1)*numX; 
                counter++;
                vertexList[v1]->addEdge(v2, edgeweight);
                vertexList[v2]->addEdge(v1, edgeweight);
            }
        }
    }
    numEdges = counter;
}