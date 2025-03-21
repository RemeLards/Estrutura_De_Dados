
#ifndef _NODE_LIST_H_
#define _NODE_LIST_H_

typedef void* data_type;

typedef struct Node Node;

Node *node_construct_list(data_type value, Node *prev, Node *next);
void node_destroy_list(Node *n,void(*data_destroy_fn)(void* data));
void node_print_list(Node *l, void (*print_fn)(data_type));

#endif