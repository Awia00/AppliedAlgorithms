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
    else if(by->weight < heap[heapIndex].weight) // update
    {
        heap[heapIndex].weight = by->weight;
        heap[heapIndex].edge = by;
        
        moveUp(heapIndex, heap[heapIndex].weight);
    }
}

Edge* DaryHeap::pickTop()
{
    swap(0, --last);
    Edge* toReturn = heap[last].edge;
    vertexToHeapIndex[heap[last].id] = -1;

    // sink
    
    if(last > 1)
    {
        long currentW;
        long choosenChild = 0;
        long index = 0;
        long children;
        long w = heap[0].weight; // YOU NEED THIS!
        long limit;

        while(choosenChild != -1)
        {
            swap(index, choosenChild);
            index = choosenChild;
            children = getChildArray(index);
            
            choosenChild = -1;
            currentW = 10000;
            limit = children + d; limit = limit < last ? limit : last;
            for(int i = children; i < limit; i++)
            {
                if(heap[i].weight < w && heap[i].weight < currentW)
                {
                    currentW = heap[i].weight;
                    choosenChild = i;
                }
            }
        }
    }

    return toReturn;
}

void DaryHeap::print()
{
    cout << "Heap with last: " << last << endl;
    for(int i = 0; i<last; i++)
    {
        cout << "Element at " << i << ": " << endl;
        cout << "  weight: " << heap[i].weight << endl;
        cout << "  V-id: " << heap[i].id << endl;
        cout << "  table: " << vertexToHeapIndex[heap[i].id] << endl;
    }
    cout << endl;
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

long DaryHeap::getChildArray(long index)
{
    return d*index+1;
}

void DaryHeap::swap(long from, long to)
{
    Node tmp = heap[from];
    heap[from] = heap[to];
    heap[to] = tmp;

    vertexToHeapIndex[heap[to].id] = to;
    vertexToHeapIndex[heap[from].id] = from;
}