#ifndef _PRIORITY_QUEUE_VECTOR_H_
#define _PRIORITY_QUEUE_VECTOR_H_

#include "vector.h"

typedef struct PriorityQueue PriorityQueue;

PriorityQueue *pq_constructor(int cmp_fn(const void *, const void *));
void pq_push(PriorityQueue *pq, void *data);
void *pq_pop(PriorityQueue *pq);
int pq_size(PriorityQueue *pq);
void pq_destroy(PriorityQueue *pq);

#endif