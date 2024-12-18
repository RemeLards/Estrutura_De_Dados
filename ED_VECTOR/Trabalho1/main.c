
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deque.h"
#include "heap.h"
#include "vector.h"

#define MAX_PROGRAM_NAME 64
#define MAX_TYPE_NAME 4

enum program_order
{
    SO = 0,
    USER,
};

typedef struct program
{
    char* name;
    char* type;
    int priority;
    int load;
    int last_run_iteration;
    int initial_run_iteration;
}PROGRAM;

const char* rt_str    = "RT";
const char* so_str    = "SO";
const char* user_str  = "USER";

int max_heap_so_cmp_fn(const void* a, const void* b)
{
    PROGRAM* program_a = (PROGRAM*) a;
    PROGRAM* program_b = (PROGRAM*) b;

    int priority_diff = program_a->priority - program_b->priority;
    if ( priority_diff != 0 ) return priority_diff; // Compares Priority

    int load_diff = program_a->load - program_b->load;
    if ( load_diff != 0) return -load_diff; // Compares who is lighter

    return -strcmp(program_a->name,program_b->name); // Compares its names lexicographically
}
int max_heap_user_cmp_fn(const void* a, const void* b)
{
    PROGRAM* program_a = (PROGRAM*) a;
    PROGRAM* program_b = (PROGRAM*) b;

    int priority_diff = program_a->priority - program_b->priority;
    if ( priority_diff != 0 ) return priority_diff; // Compares Priority

        int last_run_iteration_diff = program_a->last_run_iteration - program_b->last_run_iteration;
    if ( last_run_iteration_diff != 0 ) return -last_run_iteration_diff; // Compares who is inactive for more time

    return -strcmp(program_a->name,program_b->name); // Compares its names lexicographically
}
int min_heap_cmp_fn(const void* a, const void* b)
{
    PROGRAM* program_a = (PROGRAM*) a;
    PROGRAM* program_b = (PROGRAM*) b;

    int initial_run_iteration_diff = program_a->initial_run_iteration - program_b->initial_run_iteration;
    if ( initial_run_iteration_diff != 0 ) return -initial_run_iteration_diff; // Compares Priority

    return strcmp(program_a->name,program_b->name); // Compares its names lexicographically// Compares its names lexicographically
}

PROGRAM* program_constructor()
{
    PROGRAM* program                = calloc(1,                          sizeof(PROGRAM));
    program->name                   = calloc(1, (MAX_PROGRAM_NAME + 1) * sizeof(char));
    program->type                   = calloc(1, (MAX_TYPE_NAME + 1)    * sizeof(char));
    program->priority               = 0;
    program->load                   = 0;
    program->last_run_iteration     = -1;
    program->initial_run_iteration  = 0;

    if ( program->name[MAX_PROGRAM_NAME] != '\0')program->name[MAX_PROGRAM_NAME] = '\0';
    return program;
}
void program_destructor(PROGRAM* program)
{
    free(program->name);
    free(program->type);
    free(program);
}

int main()
{
    int num_programs;

    Deque* real_time_program_queue = deque_construct();
    Heap* so_program_queue = heap_constructor(max_heap_so_cmp_fn);
    Heap* user_program_queue = heap_constructor(max_heap_user_cmp_fn);
    Vector* delayed_programs = vector_construct();

    scanf("%d", &num_programs);

    for (int i = 0; i < num_programs; i++)
    {
        PROGRAM* program = program_constructor();
        scanf("%s %s %d %d %d", program->name, program->type, &(program->priority), &(program->load), &(program->initial_run_iteration) );

        if ( program-> initial_run_iteration == 0 )
        {
            if ( strcmp(program->type,rt_str ) == 0) deque_push_back(real_time_program_queue, (void*)program );
            else if ( strcmp(program->type,so_str ) == 0) heap_push(so_program_queue, (void*)program);
            else if ( strcmp(program->type,user_str) == 0 ) heap_push(user_program_queue, (void*)program);
        }

        else 
        {
            if ( program-> initial_run_iteration > 0 ) vector_push_back(delayed_programs, (void*)program);
        }

    }

    int program_priority = SO;
    int i = 0;
    //While there's programs left
    while(  ( deque_size(real_time_program_queue) != 0 ) ||
            ( heap_size(so_program_queue) != 0 ) ||
            ( heap_size(user_program_queue) != 0) ||
            ( vector_size(delayed_programs) != 0)  ) 
    {
        // printf("\nRUN : %d\n",i);
        if ( vector_size(delayed_programs) > 0 ) // FIX : MORE THAN ONE PROGRAM CAN ENTER THE IN EXECUTION PIPELINE
        {
            for ( int j = 0; j < vector_size(delayed_programs); j++)
            {
                PROGRAM* program = vector_get(delayed_programs,j);

                if ( program -> initial_run_iteration == i )
                {
                    program = vector_remove(delayed_programs,j);
                    j--;

                    // printf("%s %d [RT REMOVE DELAYED]\n",program->name,program->initial_run_iteration); 

                    if ( strcmp(program->type,rt_str ) == 0) deque_push_back(real_time_program_queue, (void*)program );
                    else if ( strcmp(program->type,so_str ) == 0) heap_push(so_program_queue, (void*)program);
                    else if ( strcmp(program->type,user_str) == 0 ) heap_push(user_program_queue, (void*)program);

                    // printf("%s %d [RT INSERT]\n",program->name,program->initial_run_iteration); 
                }
            }

        }

        if ( deque_size(real_time_program_queue) > 0) 
        {
            // printf("[RT]\n");
            
            PROGRAM* program = deque_pop_front(real_time_program_queue);
            program->last_run_iteration = i;
            // printf("%s %d [RT LOAD]\n",program->name,program->load-1); 

            if ( --(program->load) <= 0 ) 
            {
                printf("%s %d\n",program->name,program->last_run_iteration);
                // printf("%s %d [RT FREE]\n",program->name,program->last_run_iteration);
                program_destructor(program);
            }
            
            else deque_push_back(real_time_program_queue,program);
        }


        else
        {
            if ( program_priority == SO && heap_size(so_program_queue) <= 0 ) program_priority = USER;
            else if ( program_priority == USER && heap_size(user_program_queue) <= 0 ) program_priority = SO;

            if ( program_priority == SO )
            {
                program_priority = USER;

                if ( heap_size(so_program_queue) > 0 )
                {
                    // printf("[SO]\n");
                    PROGRAM* program = heap_pop(so_program_queue);
                    program->last_run_iteration = i;
                    // printf("%s %d [SO LOAD]\n",program->name,program->load-1); 
                    
                    
                    if ( --(program->load) <= 0 )
                    {
                        printf("%s %d\n",program->name,program->last_run_iteration);
                        // printf("%s %d [SO FREE]\n",program->name,program->last_run_iteration);
                        program_destructor(program);
                    }
                    
                    else heap_push(so_program_queue,program); // This way it gets ordered in priority queue

                }
            }

            else if ( program_priority == USER )
            {
                program_priority = SO;

                if ( heap_size(user_program_queue) > 0 )
                {
                    // printf("[USER]\n");
                    PROGRAM* program = heap_pop(user_program_queue);
                    program->last_run_iteration = i;
                    // printf("%s %d [USER LOAD]\n",program->name,program->load-1); 
                    

                    if ( --(program->load) <= 0 )
                    {
                        printf("%s %d\n",program->name,program->last_run_iteration);
                        // printf("%s %d [USER FREE]\n",program->name,program->last_run_iteration);
                        program_destructor(program);
                    }
                    
                    else heap_push(user_program_queue,program); // This way it gets ordered in priority queue 

                }
                
            }
        }

        // printf("\n\nDelayed size: %d [DELAYED]\n",heap_size(delayed_programs));
        // printf("Deque size : %d [RT]\n",deque_size(real_time_program_queue));
        // printf("Heap SO size : %d [SO]\n",heap_size(so_program_queue));
        // printf("User size : %d [USER]\n\n",heap_size(user_program_queue));

        // for(int j = 0; j < vector_size(delayed_programs); j++)
        // {
        //     PROGRAM* program = vector_get(delayed_programs,j);
        //     printf("%s %d [VECTOR DELAYED ORDER]\n",program->name,program->initial_run_iteration);
        // }

        i++;
    }

    deque_destroy(real_time_program_queue);
    heap_destroy(so_program_queue);
    heap_destroy(user_program_queue);
    vector_destroy(delayed_programs);

    return 0;
}
