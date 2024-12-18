#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

const char* enqueue = "ENQUEUE" ;
const char* dequeue = "DEQUEUE";
int main()
{
    int n;
    char command[30];
    Queue* q = queue_constructor();

    scanf("%d\n",&n);

    for(int i = 0; i < n; i++)
    {
        scanf("%s ", command);

        if ( strcmp(command,enqueue) == 0 )
        {
            char* str = (char*) malloc(30 * sizeof(char));
            scanf("%s\n",str) ;
            queue_enqueue(q,(void*)str);
        }
        
        if ( strcmp(command,dequeue) == 0 )
        {
            char* str = (char*)queue_dequeue(q);
            printf("%s\n",str);
            free(str);
        }
    }

    queue_destructor(q);

    return 0;
}
