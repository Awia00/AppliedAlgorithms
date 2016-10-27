#include "disjointsetarray.h"

void DisjointSetArray::setUnion(int v1, int v2){
    int parent1 = find(v1);
    int parent2 = find(v2);
    if(ranks[parent1] > ranks[parent2])
    {
        parents[parent2] = parent1;
    }
    else if(ranks[parent1] < ranks[parent2])
    {
        parents[parent1] = parent2;
    }
    else
    {
        parents[parent1] = parent2;
        ranks[parent2]++;
    }
}

void DisjointSetArray::setUnionParents(int parent1, int parent2){
    if(ranks[parent1] > ranks[parent2])
    {
        parents[parent2] = parent1;
    }
    else if(ranks[parent1] < ranks[parent2])
    {
        parents[parent1] = parent2;
    }
    else
    {
        parents[parent1] = parent2;
        ranks[parent2]++;
    }
}

int DisjointSetArray::find(int id) {
            
    int first = parents[id];
    int prev = first;
    while(parents[prev] != prev)
    {
        prev = parents[prev];
    }
    
    if(parents[first] == prev) // if the lenght was 0 or 1
        return prev;
        
    int leader = prev;
    prev = first;
    // path compression
    int newParent;
    while(parents[prev] != prev)
    {
        newParent = parents[prev];
        parents[prev] = leader;
        prev = newParent;
    }
    return leader;
}