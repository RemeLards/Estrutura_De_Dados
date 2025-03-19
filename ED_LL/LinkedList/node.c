#include "node.h"
#include <stdlib.h>

Node* node_construct(data_type value, Node* next)
{
  Node* node = malloc(sizeof(Node));
  node -> value = value;
  node -> next = next;
  return node;
}

data_type node_value(Node* node)
{
 return node->value;
}

Node* node_next(Node* node)
{
 return node->next;
}

void node_destroy(Node* node)
{
  free(node);
}
