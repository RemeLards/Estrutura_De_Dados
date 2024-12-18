#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

const char* push = "PUSH" ;
const char* pop = "POP";
int main()
{
    int n;
    char command[30];
    Stack* s = stack_constructor();

    scanf("%d\n",&n);

    for(int i = 0; i < n; i++)
    {
        scanf("%s ", command);

        if ( strcmp(command,push) == 0 )
        {
            char* str = (char*) malloc(sizeof(char));
            scanf("%s\n",str) ;
            stack_push(s,(void*)str);
        }
        
        if ( strcmp(command,pop) == 0 )
        {
            char* str = (char*)stack_pop(s);
            printf("%s\n",str);
            free(str);
        }
    }

    stack_destructor(s);

    return 0;
}
