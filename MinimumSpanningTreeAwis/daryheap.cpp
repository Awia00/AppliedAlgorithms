#include "daryheap.h"
#include "graph.h"
#include <iostream>

using namespace std;


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
        long insert = last;
        long parent = getParent(insert);
        if(insert > 0)
        {
            while(weights[vertexIds[parent]] > w)
            {
                cout << "insert" << endl;
                swap(insert, parent);
                insert = parent;
                parent = getParent(insert);
                if(parent < 0)
                {
                    break;
                }
            }
        }
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
        
        long parent = getParent(insert);
        if(insert > 0)
        {
            while(weights[vertexIds[parent]] > w)
            {
                if(parent < 0)
                {
                    break;
                }
                cout << "decrease" << endl;
                swap(insert, parent);
                insert = parent;
                parent = getParent(insert);
            }
        }
    }
}

Edge* DaryHeap::pickTop()
{
    swap(0, last-1);
    long insert = 0;
    long child1 = getFirstChild(insert);
    long child2 = getSecondChild(insert);
    long w = weights[vertexIds[insert]];
    if(last > 1)
    {
        while(w > weights[vertexIds[child1]] || w > weights[vertexIds[child2]])
        {
            cout << "picktop" << endl;
            if( weights[vertexIds[child1]] < weights[vertexIds[child2]])
            {
                swap(insert, child1);
                insert = child1;
            }
            else
            {
                swap(insert, child2);
                insert = child2;
            }

            if( getFirstChild(insert) >= last && getSecondChild(insert) >= last)
            {
                cout << "break" << endl;
                break;
            }
            cout << "children" << endl;
            child1 = getFirstChild(insert);
            child2 = getSecondChild(insert);
        }
    }
    cout << "edge" << endl;
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