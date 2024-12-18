#include <stdio.h>
#include <stdlib.h>
#include "deque.h"

#define INITIAL_ALLOCATED_SIZE 10
#define GROWTH_RATE 2

struct Deque
{
    void** data;
    int size;
    int allocated;
    int start;
    int end;
};

Deque* deque_construct()
{
    Deque* f = ( Deque* ) calloc (1,sizeof(Deque));
    f->data = calloc(INITIAL_ALLOCATED_SIZE,sizeof(void*));
    f->size = 0;
    f->allocated = INITIAL_ALLOCATED_SIZE;
    f->start = 0;
    f->end = 0;

    return f;
}

void deque_push_back(Deque *f, void* item)
{
    if ( f != NULL )
    {
        if ( ++(f->size) >= f->allocated)
        {
            f->allocated = GROWTH_RATE * f->size;
            f->data = realloc( (void*)f->data, f->allocated * sizeof(void*));
            
            if ( f->start >= f->end )
            {
                int i = 0;
                for ( i = 0; i < f->end; i++)
                {
                    f->data[f->allocated/GROWTH_RATE + i] = f->data[i];
                }

                f->end = f->allocated/GROWTH_RATE + i;
            }
        }

        f->data[f->end] = item;
        f->end = (f->end + 1 + f->allocated) % f->allocated;
    }
}

void deque_push_front(Deque *f, void* item)
{
    if ( f != NULL )
    {
        if ( ++(f->size) >= f->allocated)
        {
            f->allocated = GROWTH_RATE * f->size;
            f->data = realloc( (void*)f->data, f->allocated * sizeof(void*));
            
            if ( f->start >= f->end )
            {
                int i = 0;
                for ( i = 0; i < f->end; i++)
                {
                    f->data[f->allocated/GROWTH_RATE + i] = f->data[i];
                }

                f->end = f->allocated/GROWTH_RATE + i;
            }
        }

        f->start = (f->start-1+f->allocated) % f->allocated;
        f->data[f->start] = item;
    }
}

void* deque_pop_back(Deque *f)
{
    if ( f != NULL )
    {
        if ( f->size > 0)
        {
            f->end = (f->end -1 + f->allocated) % f->allocated;
            void* item = f->data[f->end];
            f->size--;
            
            return item;
        }

    }

    return NULL;
}

void* deque_pop_front(Deque *f)
{
    if ( f != NULL )
    {
        if ( f->size > 0)
        {
            void* item = f->data[f->start];
            f->start = (f->start +1 + f->allocated) % f->allocated;
            f->size--;
            
            return item;
        }

    }

    return NULL;
}

void* deque_get(Deque* f, int idx)
{
    if ( f != NULL )
    {
        return f->data[ (f->start + idx) % f->allocated ];
    }

    return NULL;
}

void deque_destroy(Deque *f)
{
    free(f->data);
    free(f);
}

int deque_size(Deque *f)
{
    if ( f != NULL ) return f->size;

    return -1;
}