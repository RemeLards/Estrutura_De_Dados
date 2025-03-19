#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "forward_list.h"

void print_data(data_type data)
{
    printf("%d",(int)data);
}

int main()
{
    int num_instructions,val;
    scanf("%d", &num_instructions);

    ForwardList *list = forward_list_construct();

    for( int i = 0; i < num_instructions; i++)
    {
        scanf("%d", &val);
        forward_list_push_front(list,val);
    }

    forward_list_print(list, print_data);

    forward_list_destroy(list);

    return 0;
}
