#include "daryheap.h"
#include "graph.h"
#include <iostream>

using namespace std;

void DaryHeap::moveUp(long index, long w)
{
    long parent = getParent(index);
    if(index > 0)
    {
        while(heap[parent].weight > w)
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
    long heapIndex = vertexToHeapIndex[to->id];
    if(heapIndex == -1) // insert
    {
        Node newNode = Node(to->id, by->weight, by);
        heap[last] = newNode;
        vertexToHeapIndex[to->id] = last;
        
        moveUp(last, newNode.weight);
        last++;
    }
    else if(by->weight < heap[heapIndex].weight)
    {
        heap[heapIndex].weight = by->weight;
        heap[heapIndex].edge = by;
        
        moveUp(heapIndex, heap[heapIndex].weight);
    }
}

Edge* DaryHeap::pickTop()
{
    swap(0, last-1);
    long index = 0;
    long w = heap[0].weight;
    if(last >= 1)
    {
        Node* children = getChildArray(index);
        long currentW = 10000;
        long choosenChild = -1;
        while(children != NULL)
        {
            for(int i = 0; i<d; i++)
            {
                if(index + i + 1 >= last)
                {
                    break; 
                }
                else if(children[i].weight < w && children[i].weight < currentW)
                {
                    currentW = children[i].weight;
                    choosenChild = i;
                }   
            }
            if(choosenChild == -1) // it shall notmove down anymore
            {
                goto stop;
            }
            index = d*index + choosenChild + 1;
            children = getChildArray(index);
            
            choosenChild = -1;
            currentW = 10000;
        }
    }
    stop: 
    vertexToHeapIndex[last-1] = -1;
    return heap[--last].edge;
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

Node* DaryHeap::getChildArray(long index)
{
    return heap + d*index+1;
}

void DaryHeap::swap(long from, long to)
{
    Node tmp = heap[from];
    heap[from] = heap[to];
    heap[to] = tmp;

    long tmpId = vertexToHeapIndex[from];
    vertexToHeapIndex[from] = vertexToHeapIndex[to];
    vertexToHeapIndex[to] = tmpId;
}