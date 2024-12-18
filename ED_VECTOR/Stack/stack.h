#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdlib.h>
#include <stdio.h>

#include "vector.h"

typedef struct stack Stack;

Stack* stack_constructor();
void stack_destructor(Stack* s);
int stack_size(Stack* s);
void stack_push(Stack* s, void* data);
void* stack_pop(Stack* s);


#endif