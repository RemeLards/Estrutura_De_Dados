#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "priority_queue_vector.h"


typedef struct SO_PROCESS
{
    char name[32];
    char category[32];
    int identifier;
    int priority;
} SO_PROCESS;

int cmp_function_priority(const void* a, const void* b)
{
    SO_PROCESS* process_a = (SO_PROCESS*) a;
    SO_PROCESS* process_b = (SO_PROCESS*) b;

    if ( process_a -> priority > process_b -> priority )
    {
        return 1;
    }
    else if ( process_a -> priority < process_b -> priority )
    {
        return -1;
    }

    return 0;
}

int main()
{
    int n;

    scanf("%d\n",&n);

    PriorityQueue* pq = pq_constructor(cmp_function_priority);

    for ( int i = 0; i < n; i++ )
    {
        SO_PROCESS* process = ( SO_PROCESS* ) malloc (sizeof(SO_PROCESS));
        scanf("%s %s %d %d", process->name, process->category, &(process->identifier), &(process->priority));

        pq_push(pq, (void*)process);
    }

    for ( int i = 0; i < n; i++ )
    {
        SO_PROCESS* process = pq_pop(pq);
        printf("%s %s %d %d\n",process->name, process->category, process->identifier, process->priority);
        free(process);
    }

    pq_destroy(pq);



    return 0;
}
