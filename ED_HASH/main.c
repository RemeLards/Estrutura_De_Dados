
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "vector.h"


int hash_str(HashTable *h, void *data)
{
    char *str = (char *)data;

    long hash_val = 0;
    int base = 127;

    for (size_t i = 0; i < strlen(str); i++)
        hash_val = (base * hash_val + str[i]) % hash_table_size(h);

    return hash_val;
}

int cmp_hash_item_str(void *a, void *b)
{
    HashTableItem* item_a = a;
    HashTableItem* item_b = b;

    return strcmp(item_a->key, item_b->key);
}

int cmp_str(void *a, void *b)
{
    return strcmp((char *)a, (char *)b);
}


int main()
{
    HashTable *h = hash_table_construct(11, hash_str, cmp_str);

    int n;
    
    scanf("%d", &n);

    for(int i = 0; i < n; i++) {
        char *name = malloc(sizeof(char) * 100);
        scanf("%s", name);
        void* key_status = hash_table_get(h,name);

        if ( key_status == NULL )
        {
            int* frequency = malloc(sizeof(int));
            *frequency=1;
            hash_table_set(h, name, frequency);
        }
        else
        {
            (*((int*)key_status))++;
            free(name);
        }
    }
    
    Vector *v = hash_to_vector(h);
    vector_sort(v,cmp_hash_item_str);

    int size = vector_size(v);
    for(int i = 0; i < size; i++) {
        void *pair = vector_pop_front(v);
        printf("%s %d\n", (char *)((HashTableItem *)pair)->key, *(int *)((HashTableItem *)pair)->val);
    }

    vector_destroy(v);
    hash_table_destroy(h);

    return 0;
}
