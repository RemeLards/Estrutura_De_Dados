
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deque.h"

const char* pop_front   = "POP_FRONT";
const char* pop_back    = "POP_BACK";
const char* push_front  = "PUSH_FRONT";
const char* push_back   = "PUSH_BACK";
const char* get_command   = "GET";

int main()
{
    int n;

    char comando[30];
    int item;

    Deque *dq = deque_construct();

    scanf("%d", &n);

    for (int i = 0; i < n; i++)
    {
        char input[50];
        fgets(input, sizeof(input), stdin); // Read the entire line, including commands and optional arguments

        scanf("%s", comando);

        if ( strcmp( comando, pop_front) == 0 ) printf("%d\n",deque_pop_front(dq));
        

        if ( strcmp( comando, pop_back) == 0 ) printf("%d\n",deque_pop_back(dq));

        if ( strcmp( comando, push_front) == 0 )
        {
            scanf("%d", &item);
            deque_push_front(dq,item); 
        }

        if ( strcmp( comando, push_back) == 0 )
        {
            scanf("%d", &item);
            deque_push_back(dq,item); 
            
        }

        if ( strcmp( comando, get_command) == 0 )
        {
            scanf("%d", &item);
            printf("%d\n",deque_get(dq,item)); 
            
        }
    }

    deque_destroy(dq);

    return 0;
}
