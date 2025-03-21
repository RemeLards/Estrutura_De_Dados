#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include "node_list.h"

typedef struct Node
{
    data_type value;
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct List
{
    Node *head;
    Node *last;
    int size;
} List;

typedef struct ListIterator
{
    Node *current;
} ListIterator;



List* list_construct()
{
    List* dll = (List*) malloc (sizeof(List));
    dll->head = NULL;
    dll->last = NULL;
    dll->size = 0;

    return dll;
}


void list_destroy(List* dll,void(*data_destroy_fn)(void* data))
{
    if( dll != NULL )
    {
        while(dll->head != NULL)
        {
            if(dll->head->next != NULL)
            {
                dll->head = dll->head->next;
                node_destroy_list(dll->head->prev,data_destroy_fn);
            }
            else
            {
                node_destroy_list(dll->head,data_destroy_fn);
                dll->head = NULL;
            }
    
        }

        free(dll);
    }
}


void list_push_front(List* dll, data_type data)
{
    if ( dll != NULL )
    {
        if ( dll->size == 0 )
        {
            dll->head = node_construct_list(data,NULL,NULL);
            dll->last = dll->head;
        }

        else
        {
            dll->head = node_construct_list(data,NULL,dll->head);
            dll->head->next->prev = dll->head;
        }

        dll->size++;
    } 
}


data_type list_pop_front(List* dll)
{
    data_type val = NULL;

    if ( dll != NULL )
    {
        if ( dll-> head != NULL )
        {
            val = dll->head->value;

            if ( dll->head->next != NULL )
            {
                dll->head = dll->head->next;
                node_destroy_list(dll->head->prev,NULL);
                dll->head->prev = NULL;
            }

            else
            {
                node_destroy_list(dll->head,NULL);
                dll->head = NULL;
                dll->last = NULL;
            }

            dll->size--;
        }

    }

    return val;
}


void list_push_back(List* dll, data_type data)
{
    if ( dll != NULL )
    {
        if(dll->size == 0)
        {
            dll->last = node_construct_list(data,NULL,NULL);
            dll->head = dll->last;
        }
        else
        {
            dll->last->next = node_construct_list(data,dll->last,NULL);
            dll->last = dll->last->next;
        }

        dll->size++;
    }
}


data_type list_pop_back(List* dll)
{
    data_type val = NULL;

    if ( dll != NULL )
    {
        if ( dll->last != NULL )
        {
            val = dll->last->value;
            
            if ( dll->last->prev != NULL )
            {
                dll->last = dll->last->prev;
                node_destroy_list(dll->last->next,NULL);
                dll->last->next = NULL;
            }

            else
            {
                node_destroy_list(dll->last,NULL);
                dll->last = NULL;
                dll->head = NULL;
            }

            dll->size--;
        }
    }

    return val;
}


// data_type list_pop_item(List* dll,int(*data_cmp_fn)(void* list_item,void* item), void* item)
// {
//     data_type val = NULL;

//     if ( dll != NULL )
//     {
//         ListIterator* it = list_front_iterator(dll);

//         while(it->current != NULL)
//         {
//             void* data = list_iterator_next(it);

//             if ( data != NULL )
//             {
//                 if ( data_cmp_fn(data, item) == 0)
//                 {
//                     void* temp_data = NULL;
//                     if( dll-> head == it -> current)temp_data=list_pop_front(dll);
//                     else if( dll->last == it -> current)temp_data=list_pop_back(dll);
//                     else
//                     {
//                         it->current->prev->next = it->current->next;
//                         temp_data = it->current->value;
//                         node_destroy_list(it->current,NULL);
//                         dll->size--;   
//                     } 
//                     list_iterator_destroy(it);
//                     return  (data_type)temp_data;
//                 }
//             }
//         }
//         list_iterator_destroy(it);
//     }

//     return val;   
// }


void list_print(List* dll, void (*print_fn)(data_type))
{
    if ( dll != NULL )
    {
        Node* head_copy = dll->head;
        printf("[");
        while( head_copy != NULL )
        {
            print_fn(head_copy->value);

            if( head_copy->next != NULL )
            {
                printf(", ");
            }

            head_copy = head_copy->next;
        }
        printf("]");
    }
}


void list_print_reverse(List *dll, void (*print_fn)(data_type))
{
    if ( dll != NULL )
    {
        Node* tail_copy = dll->last;
        printf("[");
        while( tail_copy != NULL )
        {
            print_fn(tail_copy->value);

            if( tail_copy->prev != NULL )
            {
                printf(", ");
            }

            tail_copy = tail_copy->prev;
        }
        printf("]");
    }  
}


int list_size(List* dll)
{
    return dll->size;
}


ListIterator* list_front_iterator(List* list)
{
    ListIterator* front_it = (ListIterator*) malloc (sizeof(ListIterator));
    front_it->current = list->head;

    return front_it;
}


ListIterator* list_back_iterator(List* list)
{
    ListIterator* back_it = (ListIterator*) malloc (sizeof(ListIterator));
    back_it->current = list->last;

    return back_it;
}


int list_iterator_is_over(ListIterator* it)
{
    if ( it != NULL )
    {
        if ( it -> current != NULL )
        {
            return 0;
        }

    }

    return 1;
}


data_type list_iterator_previous(ListIterator* it)
{
    data_type val = NULL;

    if ( it != NULL )
    {
        if ( it -> current != NULL )
        {
            val = it->current->value;
            it->current = it->current->prev;
        }
    }

    return val;
}


data_type list_iterator_next(ListIterator* it)
{
    data_type val = NULL;

    if ( it != NULL )
    {
        if ( it -> current != NULL )
        {
            val = it->current->value;
            it->current = it->current->next;
        }
    }

    return val;
}


void list_iterator_destroy(ListIterator* it)
{
    free(it);
}


