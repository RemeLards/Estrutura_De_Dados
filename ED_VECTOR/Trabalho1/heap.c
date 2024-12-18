#include <stdlib.h>
#include "vector.h"
#include "heap.h"

struct Heap
{
    Vector* v;
    int (*cmp_fn)(const void *, const void *);
};

Heap* heap_constructor(int (*cmp_fn)(const void *, const void *))
{
    Heap* heap = (Heap*) calloc (1,sizeof(Heap));
    heap->v = vector_construct();
    heap->cmp_fn = cmp_fn;

    return heap;
}

void heap_push(Heap *heap, void *data)
{
    if ( heap != NULL )
    {
        if ( data != NULL )
        {
            vector_push_back(heap->v,data);

            for ( int i = heap_size(heap)-1; i > 0;)
            {
                // Next Parent
                i = (i-1)/2;

                if (i >= 0)
                {
                    if ( heap->cmp_fn(data,heap->v->data[i]) >= 1 )
                    {
                        //Swap Parent with Children
                        vector_swap(heap->v,vector_find(heap->v,data,heap->cmp_fn),vector_find(heap->v,heap->v->data[i],heap->cmp_fn));
                    }
                    else break; // Children isn't higher priority
                }
            }
        }
    }
}

int heap_size(Heap *heap)
{
    if ( heap != NULL )
    {
        return vector_size(heap->v);
    }

    return -1; 
}

void* heap_pop(Heap *heap)
{
    if ( heap !=  NULL )
    {
        if (heap_size(heap) > 0)
        {

            vector_swap(heap->v,0,heap_size(heap)-1);

            void* data = vector_pop_back(heap->v);

            for( int i = 0; i < heap_size(heap); )
            {
                int left_children = 2*i + 1;
                int right_children = 2*i + 2;

                if(right_children < heap_size(heap))
                {
                    if ( !( ( heap->cmp_fn(vector_get(heap->v,i) ,vector_get(heap->v,left_children)) >= 1) &&
                            ( heap->cmp_fn(vector_get(heap->v,i) ,vector_get(heap->v,right_children)) >= 1 ) ) )
                    {

                        if ( heap->cmp_fn(vector_get(heap->v,left_children) ,vector_get(heap->v,right_children)) >= 1 )
                        {
                            vector_swap(heap->v,i,left_children);
                            i = left_children ;
                        }

                        else
                        {
                            vector_swap(heap->v,i,right_children);
                            i = right_children ;
                        }
                    }

                    else break;
                }
                else if(left_children < heap_size(heap))
                {
                    if ( !( heap->cmp_fn(vector_get(heap->v,i) ,vector_get(heap->v,left_children)) >= 1) )
                    {
                        vector_swap(heap->v,i,left_children);
                        i = left_children ;
                    }
                    
                    else break;
                }
                else break; //Reached the end of the heap
            }

            return data;
        }

    }

    return NULL;
}

void heap_destroy(Heap *heap)
{
    if ( heap != NULL )
    {
        vector_destroy(heap->v);
        free(heap);
    }
}

void* heap_dumb_pop(Heap* heap)
{
    if ( heap != NULL )
    {
        return vector_pop_front(heap->v); 
    }

    return NULL;
}

void* heap_get(Heap* heap, int i)
{
    if ( heap != NULL )
    {
        return vector_get(heap->v,i);
    }

    return NULL;
}