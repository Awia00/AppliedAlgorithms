
typedef struct node {
  int x;
  int y;
  struct node *next;
} node;

node *add_node_end(node *prev, int x, int y)
{
    node *new = malloc(sizeof *new);
    new->next = 0;
    new->x = x;
    new->y = y;
    prev->next = new;
    return prev;
}

node *add_node_front(node *next, int x, int y)
{
    node *new = malloc(sizeof *new);
    new->x = x;
    new->y = y;
    new->next = next;
    return new;
}

int has_next(node *n)
{
    return !!n->next; // if the pointer is 0 inverse it, and then inverse it again
}

node *get_last(node *n)
{
    if(n)
    {
        while(has_next(n))
        {
            n = n->next;
        }
    }
    return n;
}