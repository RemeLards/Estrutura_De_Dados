#include "queue.h"

struct queue
{
    Vector* queue;
};

Queue* queue_constructor()
{
    Queue* q = (Queue *) calloc(1,sizeof(Queue));
    q->queue = vector_construct();
    
    return q;
}

void queue_destructor(Queue* q)
{
    for (int i = 0; i < vector_size(q->queue); i++)
    {
        free(q->queue->data[i]);
        q->queue->size--;
    }
    vector_destroy(q->queue);
    free(q);
}

int queue_size(Queue* q)
{
    return vector_size(q->queue);
}

void queue_enqueue(Queue* q, void* data)
{
    vector_push_back(q->queue,data);
}

void* queue_dequeue(Queue* q)
{
    void* data = vector_pop_front(q->queue);
    return data;
}