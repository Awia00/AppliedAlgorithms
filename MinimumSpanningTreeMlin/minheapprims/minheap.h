#ifndef MinHeapHeader

#define MinHeapHeader
struct HeapEdge {
    long weight;
    long vertex;
};

class MinHeap {
    private:
        HeapEdge *heap;
        long size;

        void swap(long i, long j);

        void sink(long i);

        void rise(long i);

    public:
        MinHeap(long maxSize);

        void insert(long vertex, long weight);

        HeapEdge extractMin();

        void decreaseKey(long vertex, long newWeight);

        long weightOfVertex(long vertex);

        bool any();
};
#endif