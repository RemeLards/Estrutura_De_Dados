#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include "node.h"

List* list_construct()
{
    List* dll = (List*) malloc (sizeof(List));
    dll->head = NULL;
    dll->last = NULL;
    dll->size = 0;

    return dll;
}


void list_destroy(List* dll)
{
    if( dll != NULL )
    {
        while(dll->head != NULL)
        {
            if(dll->head->next != NULL)
            {
                dll->head = dll->head->next;
                node_destroy(dll->head->prev);
            }
            else
            {
                node_destroy(dll->head);
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
            dll->head = node_construct(data,NULL,NULL);
            dll->last = dll->head;
        }

        else
        {
            dll->head = node_construct(data,NULL,dll->head);
            dll->head->next->prev = dll->head;
        }

        dll->size++;
    } 
}


data_type list_pop_front(List* dll)
{
    data_type val = 0;

    if ( dll != NULL )
    {
        if ( dll-> head != NULL )
        {
            val = dll->head->value;

            if ( dll->head->next != NULL )
            {
                dll->head = dll->head->next;
                node_destroy(dll->head->prev);
                dll->head->prev = NULL;
            }

            else
            {
                node_destroy(dll->head);
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
            dll->last = node_construct(data,NULL,NULL);
            dll->head = dll->last;
        }
        else
        {
            dll->last->next = node_construct(data,dll->last,NULL);
            dll->last = dll->last->next;
        }

        dll->size++;
    }
}


data_type list_pop_back(List* dll)
{
    data_type val = 0;

    if ( dll != NULL )
    {
        if ( dll->last != NULL )
        {
            val = dll->last->value;
            
            if ( dll->last->prev != NULL )
            {
                dll->last = dll->last->prev;
                node_destroy(dll->last->next);
                dll->last->next = NULL;
            }

            else
            {
                node_destroy(dll->last);
                dll->last = NULL;
                dll->head = NULL;
            }

            dll->size--;
        }
    }

    return val;
}


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

bool list_iterator_is_over(ListIterator* it)
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

data_type* list_iterator_previous(ListIterator* it)
{
    data_type* val = NULL;

    if ( it != NULL )
    {
        if ( it -> current != NULL )
        {
            val = &(it->current->value);
            it->current = it->current->prev;
        }
    }

    return val;
}

data_type* list_iterator_next(ListIterator* it)
{
    data_type* val = NULL;

    if ( it != NULL )
    {
        if ( it -> current != NULL )
        {
            val = &(it->current->value);
            it->current = it->current->next;
        }
    }

    return val;
}

void list_iterator_destroy(ListIterator* it)
{
    free(it);
}


