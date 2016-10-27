#include "daryheap.h"
#include "graph.h"
#include <iostream>

using namespace std;

void DaryHeap::moveUp(long index, long w)
{
    long parent = getParent(index);
    if(index > 0)
    {
        while(weights[vertexIds[parent]] > w)
        {
            swap(index, parent);
            index = parent;
            parent = getParent(index);
            if(parent < 0)
            {
                break;
            }
        }
    }
}

void DaryHeap::insert(Vertex* to, Edge* by)
{
    if(edgesTo[to->id] == NULL)
    {
        long w = by->weight;
        //insert
        vertexIds[last] = to->id;
        weights[to->id] = w;
        edgesTo[to->id] = by;
        
        // move up
        moveUp(last, w);
        last++;
    }
    else if(by->weight < weights[to->id])
    {
        long w = by->weight;
        weights[to->id] = w;
        edgesTo[to->id] = by;

        long insert = -1;
        for(int i = 0; i<=last; i++)
        {
            if(vertexIds[i] == to->id)
            {
                insert = i;
                break;
            }
        }
        
        moveUp(insert, w);
    }
}

Edge* DaryHeap::pickTop()
{
    swap(0, last-1);
    long insert = 0;
    long w = weights[vertexIds[insert]];
    if(last >= 1)
    {
        int* children = getChildArray(insert);
        long currentW = weights[vertexIds[children[0]]];
        long choosenChild = children[0];
        while(children != NULL)
        {
            for(int i = 0; i<d; i++)
            {
                if(insert + i >= last)
                {
                    children = NULL;
                    break; 
                }
                
                if(vertexIds[children[i]] < w || vertexIds[children[i]] < currentW)
                {
                    currentW = weights[vertexIds[children[i]]];
                    choosenChild = i;
                }   
            }
            w = currentW;
            insert = choosenChild + insert;
        }
    }
    return edgesTo[vertexIds[--last]];
}

long DaryHeap::getParent(long index)
{
    return (index-1)/d;
}

long DaryHeap::getFirstChild(long index)
{
    return d*index+1;
}

long DaryHeap::getSecondChild(long index)
{
    return d*index+2;
}

int* DaryHeap::getChildArray(long index)
{
    return vertexIds + d*index+1;
}

void DaryHeap::swap(long from, long to)
{
    long tmp = vertexIds[from];
    vertexIds[from] = vertexIds[to];
    vertexIds[to] = tmp;
}