#include "stack.h"

struct stack
{
    Vector* stack;
};

Stack* stack_constructor()
{
    Stack* s = (Stack *) calloc(1,sizeof(Stack));
    s->stack = vector_construct();
    
    return s;
}

void stack_destructor(Stack* s)
{
    for (int i = 0; i < vector_size(s->stack); i++)
    {
        free(s->stack->data[i]);
        s->stack->size--;
    }
    vector_destroy(s->stack);
    free(s);
}

int stack_size(Stack* s)
{
    return vector_size(s->stack);
}

void stack_push(Stack* s, void* data)
{
    vector_push_back(s->stack,data);
}

void* stack_pop(Stack* s)
{
    void* data = vector_pop_back(s->stack);
    return data;
}