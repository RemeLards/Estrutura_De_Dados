#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "node.h"
#include "list.h"

typedef struct HashTable
{
    List** bucket;
    int size;
    HashFunction hash_fn;
    CmpFunction cmp_fn;
}HashTable;


typedef struct HashTableIterator
{
    HashTable* current_hashtable;
    Node* current_dll_node;
    int bucket_index;

    
}HashTableIterator;


void hash_item_destroy(void* void_hash_item)
{
    HashTableItem* hash_item = (HashTableItem*) void_hash_item;
    free(hash_item->key);
    free(hash_item->val);
    free(hash_item);
}


int hash_cmp_key(void* void_hash_item, void* key)
{
    HashTableItem* hash_item = void_hash_item;
    return strcmp(hash_item->key,key);
}


HashTable *hash_table_construct(int table_size, HashFunction hash_fn, CmpFunction cmp_fn)
{
    HashTable* h_table = (HashTable*) malloc(sizeof(HashTable));
    h_table-> bucket = (List**) malloc(table_size*sizeof(List));
    h_table-> size = table_size;

    for(int i = 0; i < table_size; i++)
    {
        h_table->bucket[i] = list_construct();// Creates all LinkedList Holders
    }


    h_table-> hash_fn = hash_fn;
    h_table-> cmp_fn = cmp_fn; 

    return h_table;
}

// Se a chave ja existir, atualiza o valor e retorna o valor antigo para permitir desalocacao.
void* hash_table_set(HashTable* h, void* key, void* val)
{
    if ( h != NULL && key != NULL)
    {
        long long unsigned index = h->hash_fn(h,key);

        if ( h->bucket[index]->size >= 0)
        {
            if ( h->bucket[index]->size > 0 )
            {
                ListIterator* it = list_front_iterator(h->bucket[index]);

                while(it->current != NULL)
                {
                    HashTableItem* temp_h_item = list_iterator_next(it);

                    if ( temp_h_item != NULL )
                    {
                        if ( h->cmp_fn( temp_h_item->key, key) == 0)
                        {
                            // Updates Value on the item present on the LinkedList
                            void* val2return = temp_h_item->val;
                            temp_h_item->val = val;

                            list_iterator_destroy(it);
                            return (void*)val2return;
                        }
                    }
                }
                list_iterator_destroy(it);
            }
            // If the LinkedList size is 0, don't need to loop and check if the key is in it
            // If the key doesnt exists on the LinkedList and therefore in the hash table, just add it
            HashTableItem* h_item = (HashTableItem*)malloc(sizeof(HashTableItem));
            h_item->key = key;
            h_item->val = val;
            list_push_front(h->bucket[index],(void*)h_item);

        }

    }

    return NULL;   
}


// retorna o valor associado com a chave key ou NULL se ela nao existir em O(1).
void* hash_table_get(HashTable* h, void* key) // KEY is a STR
{
    if ( h != NULL && key != NULL )
    {
        long long unsigned index = h->hash_fn(h,key);
        
        if ( h->bucket[index]->size > 0)
        {
            if ( h->bucket[index]->size > 1 )
            {
                ListIterator* it = list_front_iterator(h->bucket[index]);

                while(it->current != NULL)
                {
                    HashTableItem* temp_h_item = list_iterator_next(it);

                    if ( temp_h_item != NULL )
                    {
                        if ( h->cmp_fn( temp_h_item->key, key) == 0)
                        {
                            list_iterator_destroy(it);
                            return  (void*)temp_h_item->val;
                        }
                    }
                }
                list_iterator_destroy(it);
            }

            else
            {
                if ( h->bucket[index]->head != NULL )
                {
                    HashTableItem* temp_h_item = h->bucket[index]->head->value;
                    if ( h->cmp_fn( temp_h_item->key, key) == 0 ) return (void*)temp_h_item->val;
                }
            }
        }

    }

    return NULL;

}


int hash_table_size(HashTable* h)
{
    return h->size;
}


void hash_table_destroy(HashTable* h)
{
    int size_h_table = hash_table_size(h);
    for(int i = 0;i < size_h_table; i++) //Frees each element of the table
    {
        if( h->bucket[i] != 0) list_destroy(h->bucket[i],hash_item_destroy);
    }
    free(h->bucket);
    free(h);
}


void* hash_table_pop(HashTable* h, void* key)
{
    if ( h != NULL && key != NULL )
    {
        long long unsigned index = h->hash_fn(h,key);

        if ( h->bucket[index]->size > 0 )
        {
            // HashTableItem* item = list_pop_item(h->bucket[index],hash_cmp_key,key);
            ListIterator* it = list_front_iterator(h->bucket[index]);
            Node* it_prev_node = it->current;

            while(it->current != NULL)
            {
                HashTableItem* temp_h_item = list_iterator_next(it);

                if ( temp_h_item != NULL )
                {
                    if ( strcmp(temp_h_item->key, key) == 0)
                    {
                        if ( it_prev_node->prev == NULL &&  it_prev_node == h->bucket[index]->head )list_pop_front(h->bucket[index]); // Updates HEAD                
                        else
                        {
                            if ( it_prev_node->prev != NULL ) it_prev_node->prev->next = it_prev_node->next;
                            if ( it_prev_node->next != NULL ) it_prev_node->next->prev = it_prev_node->prev;
                            node_destroy(it_prev_node,NULL);
                            h->bucket[index]->size--;
                        }
                    
                        void* stored_val = malloc(sizeof(void*));
                        memcpy (stored_val,temp_h_item->val,sizeof(int));

                        hash_item_destroy(temp_h_item);

                        list_iterator_destroy(it);
                        return stored_val;
                    }
                }
                it_prev_node = it->current;
            }

            list_iterator_destroy(it);
        } 
    }

    return NULL;
}


HashTableIterator* hash_table_iterator(HashTable *h)
{
    HashTableIterator* hash_it = (HashTableIterator*) malloc (sizeof(HashTableIterator));
    hash_it -> bucket_index = 0;
    hash_it -> current_hashtable = h;
    hash_it -> current_dll_node = hash_it->current_hashtable->bucket[hash_it->bucket_index]->head;

    return hash_it;
}

// retorna 1 se o iterador chegou ao fim da tabela hash ou 0 caso contrario
int hash_table_iterator_is_over(HashTableIterator *it)
{
    if ( it != NULL)
    {
        if ( it->current_hashtable != NULL )
        {
            if ( it->bucket_index >= it->current_hashtable->size )
            {
                return 1;
            }

            return 0;
        }

        return -2; // HashTable is NULL
    }

    return -1;// Iterator is NULL 
}

// retorna o proximo par chave valor da tabela hash
HashTableItem* hash_table_iterator_next(HashTableIterator *it)
{
    if ( it != NULL )
    {

        if ( it->current_dll_node == NULL  ) // If the first BUCKET HEAD is NULL
        {
            for (it->bucket_index++; it->bucket_index < it->current_hashtable->size; it->bucket_index++)
            {
                if ( it->current_hashtable->bucket[it->bucket_index]->head != NULL )
                {
                    it->current_dll_node =  it->current_hashtable->bucket[it->bucket_index]->head;
                    break;
                }
            }
        }

        if ( it->current_dll_node != NULL ) // It should always not be NULL, except the first BUCKET HEAD is NULL
        {
            HashTableItem* h_item_copy = (HashTableItem*) it->current_dll_node->value;
            if ( it->current_dll_node->next != NULL )
            {
                it->current_dll_node = it->current_dll_node->next;
            }

            else
            {
                for (it->bucket_index++; it->bucket_index < it->current_hashtable->size; it->bucket_index++)
                {
                    if ( it->current_hashtable->bucket[it->bucket_index]->head != NULL )
                    {
                        it->current_dll_node =  it->current_hashtable->bucket[it->bucket_index]->head;
                        break;
                    }
                }
            }
            return h_item_copy;
        }
        
    }

    return NULL;
}

// desaloca o iterador da tabela hash
void hash_table_iterator_destroy(HashTableIterator *it)
{
    free(it);
}


Vector* hash_to_vector(HashTable *h)
{
    if ( h != NULL )
    {
        Vector* v = vector_construct();
        HashTableIterator *it = hash_table_iterator(h);

        while (!hash_table_iterator_is_over(it))
        {
            HashTableItem *pair = hash_table_iterator_next(it);
            vector_push_back(v,(void*)pair);
        }
        hash_table_iterator_destroy(it);

        return v;
    }

    return NULL; 
}