#ifndef DisjointSetHeader

#define DisjointSetHeader

class DisjointSet{
    public:
        DisjointSet(long numVertices);
        long find(long id);
        void setUnion(long v1, long v2);
        
        bool sameSet(long v1, long v2) {
            return find(v1) == find(v2);
        }
    private:
        struct UF 
        {
            long parent;
            int rank;
        };
        UF *nodes;
};

#endif