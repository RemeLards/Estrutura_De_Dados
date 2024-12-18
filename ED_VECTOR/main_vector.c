#include <stdio.h>
#include <string.h>

#include "vector.h"

void destroy_int (Vector* v)
{
    free(v->data);
}

void do_nothing (Vector* v)
{
}

int main()
{

    Vector *v, *v_copy;

    v = vector_construct();

    for (int i = 0; i <= 5; i++)
    {
        vector_push_back(v, i);
    }

    vector_set(v,0,5);
    vector_set(v,2,5);

    v_copy = vector_find_all(v,5);

    printf("\n\n\n");
    for (int i = 0; i < vector_size(v_copy); i++)
    {
        printf("%d\n",vector_get(v_copy,i));
    }
    printf("\n\n\n");

    vector_destroy(v,destroy_int);
    vector_destroy(v_copy, destroy_int);

    return 0;
}
