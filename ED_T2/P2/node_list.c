
#include <stdio.h>
#include <stdlib.h>
#include "node_list.h"

typedef struct Node
{
    data_type value;
    struct Node *prev;
    struct Node *next;
} Node;

Node *node_construct_list(data_type value, Node *prev, Node *next)
{
    Node *n = (Node *)malloc(sizeof(Node));
    n->value = value;
    n->next = next;
    n->prev = prev;
    return n;
}

void node_destroy_list(Node *n,void(*data_destroy_fn)(void* data))
{
    if ( n != NULL )
    {
        if ( data_destroy_fn != NULL ) data_destroy_fn(n->value);
        if (n) free(n);
        else printf("Node was NULL\n");
    }
}

// void node_print_debug(Node *n, void (*print_fn)(data_type))
// {
//     printf("(");

//     if (n->prev == NULL)
//         printf("NULL");
//     else
//         print_fn(n->prev->value);

//     printf(", ");
//     print_fn(n->value);
//     printf(", ");

//     if (n->next == NULL)
//         printf("NULL");
//     else
//         print_fn(n->next->value);

//     printf(")");
// }

// void node_print(Node *n, void (*print_fn)(data_type))
// {
//     print_fn(n->value);
// }