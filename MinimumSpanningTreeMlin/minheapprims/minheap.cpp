#include <iostream>
#include <stdexcept>
#include "minheap.h"

using namespace std;

void MinHeap::swap(long i, long j) {
    long swap = heap[i];
    heap[i] = heap[j];
    heap[j] = swap;

    map[heap[i]] = i;
    map[heap[j]] = j;
}

void MinHeap::sink(long i) {
    while (i < size) {
        long leftChild = i*2 + 1, rightChild = i*2 + 2;

        if (leftChild < size && rightChild < size) {
            long l = heap[leftChild], 
                r = heap[rightChild],
                lWeight = weights[l],
                rWeight = weights[r];
            if (lWeight < rWeight) {
                if (weights[heap[i]] > lWeight) {
                    swap(i, leftChild);
                    i = leftChild;
                } else {
                    return;
                }
            } else {
                if (weights[heap[i]] > rWeight) {
                    swap(i, rightChild);
                    i = rightChild;
                } else {
                    return;
                }
            }
        }
        else if (leftChild < size) {
            long l = heap[leftChild], lWeight = weights[l];
            if (weights[heap[i]] > lWeight) {
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
    while (i > 0 && weights[heap[parent]] > weights[heap[i]]) {
        swap(i, parent);
        i = parent;
        parent = (i-1)/2;
    }
}

MinHeap::MinHeap(long maxSize) {
    heap = new long[maxSize];
    weights = new long[maxSize];
    map = new long[maxSize];

    for (long i = 0; i < maxSize; i++) {
        weights[i] = 10001;
        map[i] = -1;
    }

    size = 0;
}

void MinHeap::insert(long vertex, long weight) {
    weights[vertex] = weight;
    heap[size] = vertex;
    map[vertex] = size;
    rise(size++);
}

HeapEdge MinHeap::extractMin() {
    if (size < 1) { throw length_error("No elements!"); }
    
    HeapEdge min;
    min.vertex = heap[0];
    map[heap[0]] = -1;
    min.weight = weights[min.vertex];
    heap[0] = heap[--size];
    map[heap[0]] = 0;

    sink(0);

    return min;
}

void MinHeap::decreaseKey(long vertex, long newWeight) {
    if (weights[vertex] > newWeight) {
        weights[vertex] = newWeight;
        rise(map[vertex]);
    }
}

long MinHeap::weightOfVertex(long vertex) {
    return weights[vertex] > 10000 ? -1 : weights[vertex];
}