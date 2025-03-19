
#ifndef _NODE_H_
#define _NODE_H_

typedef void* data_type;

typedef struct Node
{
    data_type value;
    struct Node *left;
    struct Node *right;
} Node;

Node *node_construct(data_type value, Node *left, Node *right);
void node_destroy(Node *n,void(*data_destroy_fn)(void* data));
void node_print(Node *l, void (*print_fn)(data_type));

#endif