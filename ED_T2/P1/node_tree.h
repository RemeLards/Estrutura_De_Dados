
#ifndef _NODE_TREE_H_
#define _NODE_TREE_H_

typedef void* data_type;

typedef struct Node
{
    data_type value;
    struct Node *left;
    struct Node *right;
} Node;

Node *node_construct_tree(data_type value, Node *left, Node *right);
void node_destroy_tree(Node *n,void(*data_destroy_fn)(void* data));
void node_print_tree(Node *l, void (*print_fn)(data_type));

#endif
