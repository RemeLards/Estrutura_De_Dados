#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdlib.h>
#include <stdio.h>

#include "vector.h"

typedef struct queue Queue;

Queue* queue_constructor();
void queue_destructor(Queue* q);
int queue_size(Queue* q);
void queue_enqueue(Queue* q, void* data);
void* queue_dequeue(Queue* q);


#endif