
#include <stdio.h>
#include <stdlib.h>
#include "node_tree.h"

Node *node_construct_tree(data_type value, Node *left, Node *right)
{
    Node *n = (Node *)malloc(sizeof(Node));
    n->value = value;
    n->left = left;
    n->right = right;
    return n;
}

void node_destroy_tree(Node *n,void(*data_destroy_fn)(void* data))
{
    if ( data_destroy_fn != NULL ) data_destroy_fn(n->value);
    free(n);
}

