#include "priority_queue_vector.h"
#include "vector.h"
#include <stdlib.h>

struct PriorityQueue
{
    Vector* v;
    int (*cmp_fn)(const void *, const void *);
};

PriorityQueue *pq_constructor(int cmp_fn(const void *, const void *))
{
    PriorityQueue* pq = (PriorityQueue *) calloc(1,sizeof(PriorityQueue));
    pq -> v = vector_construct();
    pq -> cmp_fn = cmp_fn;
    return pq;
}

void pq_push(PriorityQueue *pq, void *data)
{
    if ( pq != NULL )
    {
        if ( data != NULL )
        {
            if (pq_size(pq) >= 0) // compare and insert where the data priority is higher then data[i]
            {
                // since an insertion, or in other words, the pq_size increases, not saving it's previous size
                // can cause infinite loop
                int pq_size_before_insert = pq_size(pq);
                for (int i = 0; i <= pq_size_before_insert; i++ )
                {

                    if ( i < pq_size_before_insert )
                    {                        
                        if ( pq->cmp_fn(data,pq->v->data[i]) >= 1 )
                        {
                            vector_insert(pq->v,i,data);
                            break;
                        }
                    }

                    // if the priority is lower than all items in vector, inserts at the end
                    else vector_push_back(pq->v,data);
                }

            }
            
        }

    }
}

void *pq_pop(PriorityQueue *pq)
{
    //Since the Priority Queue is put in crescent order, just pop removes the higher priority element
    if ( pq !=  NULL )
    { 
        return vector_pop_front(pq->v);
    }

    return NULL;
}

int pq_size(PriorityQueue *pq)
{
    if ( pq != NULL )
    {
        return vector_size(pq->v);
    }

    return -1;
}

void pq_destroy(PriorityQueue *pq)
{
    if ( pq != NULL )
    {
        vector_destroy(pq->v);
        free(pq);
    }
}