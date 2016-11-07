#include "disjointset.h"
#include "graph.h"

Node* DisjointSet::find(long id) {
    Node* first = nodes[id];
    Node* prev = first;
    while(prev->parent != prev)
    {
        prev = prev->parent->parent; // small loop unrolling->
    }
    
    if(first->parent == prev)
        return prev;
        
    Node* leader = prev;
    prev = first;
    // path compression
    Node* newParent;
    while(prev->parent != prev)
    {
        newParent = prev->parent;
        prev->parent = leader;
        prev = newParent;
    }
    return leader;
}

void DisjointSet::setUnion(long v1, long v2){
    Node* parent1 = find(v1);
    Node* parent2 = find(v2);
    if(parent1->rank > parent2->rank)
    {
        parent2->parent = parent1;
    }
    else if(parent1->rank < parent2->rank)
    {
        parent1->parent = parent2;
    }
    else
    {
        parent1->parent = parent2;
        parent2->rank++;
    }
}
void DisjointSet::setUnion(Node* parent1, Node* parent2){
    if(parent1->rank > parent2->rank)
    {
        parent2->parent = parent1;
    }
    else if(parent1->rank < parent2->rank)
    {
        parent1->parent = parent2;
    }
    else
    {
        parent1->parent = parent2;
        parent2->rank++;
    }
}


bool DisjointSet::sameSet(long v1, long v2)
{
    return find(v1) == find(v2);
}