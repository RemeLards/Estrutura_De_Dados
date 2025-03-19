
#include <stdio.h>
#include <stdlib.h>
#include "forward_list.h"

ForwardList* forward_list_construct()
{
	ForwardList* list = malloc(sizeof(ForwardList));
	list->head = NULL;
	list->last = NULL;
	list->size = 0;
	return list;
	
}


void forward_list_destroy(ForwardList* l)
{
	if(l != NULL)
	{
		for(int i = 0; i < forward_list_size(l); i++)
		{
			Node* head_copy = l->head;
			l->head = node_next(l->head);

			// if( head_copy -> value != NULL )
			// {
			// 	free(head_copy->value);
			// }	

			node_destroy(head_copy);

		}

		free(l);
	}
}


void forward_list_push_front(ForwardList *l, data_type data)
{
	if(l != NULL)
	{
		if ( l->size == 0)
		{
			Node* new_head = node_construct(data,l->head);
			l->head = new_head;
			l->last = l->head;
			l->size++;
		}

		else
		{
			Node* new_head = node_construct(data,l->head);
			l->head=new_head;
			l->size++;

			Node* head_copy = l->head;

			while(node_next(head_copy) != NULL && head_copy->value > node_next(head_copy)->value)
			{
				data_type temp_val = head_copy->value;

				head_copy->value = node_next(head_copy)->value;
				node_next(head_copy)->value = temp_val;

				head_copy = node_next(head_copy);
			}
		}
	}
}


void forward_list_print(ForwardList *l, void (*print_fn)(data_type))
{
	if(l != NULL)
	{
		Node* head_copy = l->head;
		for(int i = 0; i < l->size; i++)
		{
			print_fn(node_value(head_copy));
			head_copy = node_next(head_copy);
			if (i != l->size-1) printf("\n");
		}
	}
}


int forward_list_size(ForwardList *l)
{
	return l->size;
}


data_type forward_list_pop_front(ForwardList *l)
{
	data_type data = 0;
	if(l != NULL)
	{
		if( l->head != NULL )
		{
			Node *temp = l->head;
			l->head = node_next(l->head);
			data = temp->value;
			node_destroy(temp);

			l->size--;
		}
	}

	return data;
}


data_type forward_list_get(ForwardList *l, int i)
{
	data_type data = 0;
	if( l != NULL) 
	{
		if( i >= 0 && i < forward_list_size(l))
		{
			Node *temp = l->head;
			for(int j = 0; j < i; j++)
			{
				temp = temp->next;
			}

			data = temp->value;
		}

		else
		{
			printf("Indice Invalido na lista encadeada");
			exit(1);
		}
	}

	return data;
}


void forward_list_remove(ForwardList *l, data_type val)
{
	if (l != NULL)
    {
        Node *temp = l->head;
        Node *temp_back = NULL;

        while (temp != NULL)
        {
            if (temp->value == val)
            {
                Node *to_delete = temp;
                temp = node_next(temp);

                if (to_delete == l->head)
                {
                    l->head = temp;
                }
                else
                {
                    temp_back->next = temp;
                }

                node_destroy(to_delete);
                l->size--; 
            }
            else
            {
                temp_back = temp; 
                temp = node_next(temp);
            }
        }
    }
}


void forward_list_cat(ForwardList *l, ForwardList *m)
{
	if (l != NULL && m != NULL)
	{
		Node* m_head_copy = m->head;

		while(m_head_copy -> next != NULL)
		{
			forward_list_push_front(l,m_head_copy->value);
			m_head_copy = node_next(m_head_copy);
		}
		forward_list_push_front(l,m_head_copy->value);
	}
}


void forward_list_clear(ForwardList *l)
{
	if ( l != NULL )
	{
		while(l->head != NULL)
		{
			Node* temp = l->head;
			l->head = node_next(l->head);
			node_destroy(temp);
		}

		l->size = 0;
	}
}


ForwardList *forward_list_reverse(ForwardList *l)
{
	if ( l != NULL )
	{
		ForwardList* l_reversed = forward_list_construct();
		Node* l_head_copy = l->head;

		while( l_head_copy -> next != NULL )
		{
			forward_list_push_front(l_reversed,l_head_copy->value);
			l_head_copy = node_next(l_head_copy);
		}
		forward_list_push_front(l_reversed,l_head_copy->value);

		return l_reversed;
	}


	return l;
}


data_type forward_list_pop_index(ForwardList *l, int index)
{
	data_type val = 0;

	if ( l != NULL )
	{
		if ( index >= 0 && index < l->size )
		{
			int i = 0;
			Node* head_copy = l->head;
			Node* old_head = NULL;

			if( index == 0 )
			{
				l->head = head_copy->next;
			}

			else
			{
				while ( i < index )
				{
					old_head = head_copy;
					head_copy = head_copy->next;
					i++;
				}
				old_head->next = head_copy->next;
			}
			
			val = head_copy->value;

			node_destroy(head_copy);
			l->size --;
		}
	}

	return val;
}


void forward_list_push_back(ForwardList *l, data_type data)
{
	if ( l != NULL )
	{
		if ( l->size == 0)
		{
			
			Node* new_last = node_construct(data,NULL);
			l->last = new_last; // Tail is updated with new tail (in the start is NULL)
			l->head = l->last;
			l->size++;
		}

		else
		{
			Node* new_last = node_construct(data,NULL);
			l->last->next = new_last; // Old Tail points to new tail
			l->last = new_last; // Tail is updated with new tail
			l->size++;
		}
	}
}


ListIterator *list_iterator_construct(ForwardList *l)
{
	if ( l != NULL )
	{
		ListIterator* iterator = (ListIterator *) malloc(sizeof(ListIterator));
		iterator->current = l->head;

		return iterator;
	}

	return NULL;

}

void list_iterator_destroy(ListIterator *it)
{
	if ( it != NULL )
	{
		free(it); // Current Node is allocated is the ForwardList, which can be used later
	}
}

data_type* list_iterator_next(ListIterator *it)
{
	if ( it != NULL )
	{
		if ( it -> current != NULL )
		{
			data_type* val = &(it->current->value);
			it->current = it->current->next;
			return val;
		}
	}

	return NULL;
}

bool list_iterator_is_over(ListIterator *it)
{
	if ( it != NULL )
	{
		if ( it->current != NULL)
		{
			return false;
		}
	}

	return true;
}