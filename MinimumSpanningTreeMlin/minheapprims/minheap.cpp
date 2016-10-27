#include <iostream>
#include "minheap.h"

using namespace std;

void MinHeap::swap(long i, long j) {
    HeapEdge swap = heap[i];
    heap[i] = heap[j];
    heap[j] = swap;
}

void MinHeap::sink(long i) {
    while (i < size) {
        long leftChild = i*2 + 1, rightChild = i*2 + 2;
        HeapEdge l = heap[leftChild], r = heap[rightChild];

        if (leftChild < size && rightChild < size) {
            if (l.weight < r.weight) {
                if (heap[i].weight > l.weight) {
                    swap(i, leftChild);
                    i = leftChild;
                } else {
                    return;
                }
            } else {
                if (heap[i].weight > r.weight) {
                    swap(i, rightChild);
                    i = rightChild;
                } else {
                    return;
                }
            }
        }

        if (leftChild < size) {
            if (heap[i].weight > l.weight) {
                    swap(i, leftChild);
                    i = leftChild;
            }
            return;
        } else {
            return;
        }
    }
}

void MinHeap::rise(long i) {
    long parent = (i-1)/2;
    while (i > 0 && heap[parent].weight > heap[i].weight) {
        swap(i, parent);
        i = parent;
        parent = (i-1)/2;
    }
}

MinHeap::MinHeap(long maxSize) {
    heap = new HeapEdge[maxSize];
    size = 0;
}

void MinHeap::insert(long vertex, long weight) {
    HeapEdge edge;
    edge.vertex = vertex;
    edge.weight = weight;
    heap[size] = edge;
    rise(size++);
}

HeapEdge MinHeap::extractMin() {
    if (size < 1) { throw "No elements!"; }
    
    HeapEdge min = heap[0];
    heap[0] = heap[--size];

    sink(0);

    return min;
}

void MinHeap::decreaseKey(long vertex, long newWeight) {
    long i = 0;
    while (heap[i].vertex != vertex) {
        ++i;
    }
    if (heap[i].weight > newWeight) {
        heap[i].weight = newWeight;
        rise(i);
    }
}

long MinHeap::weightOfVertex(long vertex) {
    for (long i = 0; i < size; i++) {
        if (heap[i].vertex == vertex) return heap[i].weight;
    }
    return -1;
}

bool MinHeap::any() {
    return size != 0;
}