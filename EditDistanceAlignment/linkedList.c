#include<stdlib.h>
#include<stdio.h>
#include<string.h>

typedef struct node {
  int x;
  int y;
  struct node *next;
} node;

int has_next(node *n)
{
    return !!n->next; // if the pointer is 0 inverse it, and then inverse it again
}

typedef struct linked_list {
  node *root, *last;
  int size;
} linked_list;

linked_list *linked_list_new() {
  linked_list *res = malloc(sizeof *res);
  res->root = 0;
  res->last = 0;
  res->size = 0;
  return res;
}

void linked_list_free(linked_list *list) {
  node *root = list->root, *next;

  while (root) {
    next = root->next;
    free(root);
    root = next;
  }

  free(list);
}

void linked_list_add_front(linked_list *list, int x, int y) {
  node *new = malloc(sizeof *new);
  new->x = x;
  new->y = y;
  new->next = list->root;
  list->size++;
  if (list->size == 1) {
    list->last = new;
  }
  list->root = new;
}

void linked_list_add_end(linked_list *list, int x, int y) {
  node *new = malloc(sizeof *new);
  new->x = x;
  new->y = y;
  new->next = 0;
  list->size++;
  if (list->size == 1) {
    list->root = new;
  } else {
    list->last->next = new;
  }
  list->last = new;
}

void linked_list_append(linked_list *dst, linked_list *src) {
  dst->last->next = src->root;
  dst->size += src->size;
  dst->last = src->last;
}

