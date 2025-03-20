
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "vector.h"
#include "binary_tree.h"


#define MAX_ENTERPRISE_NAME 50
#define MAX_ENTERPRISE_NICKNAME 15

typedef struct Enterprise
{
    char*   name;
    char*   nickname;
    float   unit_price;
    int     n_action;
    int     n_sold;
}Enterprise;


Enterprise* enterprise_construct()
{
    Enterprise* enterprise = malloc(sizeof(Enterprise));
    enterprise->name = malloc(MAX_ENTERPRISE_NAME*sizeof(char));;
    enterprise->nickname = malloc(MAX_ENTERPRISE_NICKNAME*sizeof(char));
    enterprise->unit_price = 0;
    enterprise->n_action = 0;
    enterprise->n_sold = 0;
    return enterprise;
}


void enterprise_destroy(void* data)
{
    KeyValPair* pair = data;
    Enterprise* enterprise = pair->value;
    free(enterprise->name); 
    free(enterprise->nickname);
    // free(enterprise->unit_price);
    // free(enterprise->n_action);
    // free(enterprise->n_sold);
    free(enterprise); //The rest of the variables arent pointers
    free(pair); // Key was a pointer to enterprise->nickname
}


KeyValPair* enterprise_key_construct(Enterprise* e)
{
    KeyValPair* pair = malloc(sizeof(KeyValPair));
    pair->key = malloc(sizeof(void*));
    pair->key = &(e->unit_price);
    pair->value = malloc(sizeof(void*));
    pair->value = e->nickname;
    return pair;
}


void print_enterprise(Enterprise* enterprise)
{
    // Enterprise* enterprise = pair->value;
    printf("%s %s %f %d %d\n",enterprise->name, enterprise->nickname, enterprise->unit_price,
                              enterprise->n_action,enterprise->n_sold);
}




void read_file(FILE* f,HashTable* enterprise_ht,BinaryTree* enterprise_bt,int* n_instructions)
{

    if ( f != NULL )
    {
        char line[100];
        fgets(line, sizeof(line), f);
        sscanf(line, "%d",n_instructions); //Reads Number of instructions


        while (fgets(line, sizeof(line), f)) //Reads all Enterprises
        {

            Enterprise* enterprise = enterprise_construct();
            sscanf(line, "%s %s %f %d %d", enterprise->name, enterprise->nickname, &enterprise->unit_price,
                                           &enterprise->n_action, &enterprise->n_sold);
            
            hash_table_set(enterprise_ht,enterprise->nickname,enterprise);
            void* key = malloc(sizeof(void*));
            key = enterprise;
            binary_tree_add(enterprise_bt,enterprise_key_construct(enterprise),key);
        }

    }
    else
    {
        printf("Error opening file!\n");
        printf("Check if the file exists on your folder!\n");
        exit(1);
    }


    fclose(f);
}



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



int cmp_fn(void *key1, void *key2)
{
    KeyValPair* e1 = key1;
    KeyValPair* e2 = key2;

    if ( *((float*)(e1->key)) - *((float*)(e2->key))  != 0 ) return *((float*)(e1->key)) - *((float*)(e2->key)) ;

    return strcmp(e1->value,e2->value);
}



void key_destroy_fn(void *key)
{
    free(key);
}



void val_destroy_fn(void *val)
{
    free(val);
}



int main()
{
    int n_instructions = 0;
    char filename[65];
    scanf("%s", filename);
    FILE* f = fopen(filename,"r");
    HashTable *enterprise_ht = hash_table_construct(11, hash_str, cmp_str);
    BinaryTree *enterprise_bt = binary_tree_construct(cmp_fn, key_destroy_fn, val_destroy_fn);
    read_file(f,enterprise_ht,enterprise_bt,&n_instructions);

    for ( int i = 0; i < n_instructions; i++)
    {
        char command[65];

        scanf("%s", command);

        if ( strcmp("UPDATE",command) == 0)
        {
            char* sigla = malloc(sizeof(char)*MAX_ENTERPRISE_NICKNAME);
            float unit_price = 0;
            scanf("%s %f", sigla,&unit_price);

            //Hashtable Part
            Enterprise* enterprise = hash_table_get(enterprise_ht,sigla);
            if ( enterprise != NULL )
            {
                //Binary Tree Part

                KeyValPair* pair = malloc(sizeof(KeyValPair));
                pair->key = malloc(sizeof(void*));
                pair->key = &(enterprise->unit_price);
                pair->value = malloc(sizeof(void*));
                pair->value = enterprise->nickname;


                binary_tree_remove(enterprise_bt,pair);
                binary_tree_add(enterprise_bt,enterprise_key_construct(enterprise),enterprise);
                free(pair->key);
                free(pair->value);
                free(pair);
            }

            free(sigla);
        }

        if ( strcmp("GET",command) == 0)
        {
            char* sigla = malloc(sizeof(char)*MAX_ENTERPRISE_NICKNAME);
            scanf("%s", sigla);

            //Hashtable Part
            Enterprise* enterprise = hash_table_get(enterprise_ht,sigla);
            print_enterprise(enterprise);

            free(sigla);

        }

        if ( strcmp("RM",command) == 0)
        {
            char* sigla = malloc(sizeof(char)*MAX_ENTERPRISE_NICKNAME);
            scanf("%s", sigla);
            

            Enterprise* enterprise = hash_table_get(enterprise_ht,sigla);

            KeyValPair* pair = malloc(sizeof(KeyValPair));
            pair->key = malloc(sizeof(void*));
            pair->key = &(enterprise->unit_price);
            pair->value = malloc(sizeof(void*));
            pair->value = enterprise->nickname;

            binary_tree_remove(enterprise_bt,pair);
            hash_table_remove(enterprise_ht,enterprise->nickname,enterprise_destroy);

            free(pair->key);
            free(pair->value);
            free(pair);
            free(sigla);
        }

        if ( strcmp("INTERVAL",command) == 0)
        {
            
        }

        if ( strcmp("MIN",command) == 0)
        {
            KeyValPair* pair = binary_tree_max(enterprise_bt);
            KeyValPair* key_pair = pair->value;
            printf("%s %f\n",(char*)key_pair->value,*((float*)key_pair->value)); 
        }
        

        if ( strcmp("MAX",command) == 0)
        {
            KeyValPair* pair = binary_tree_max(enterprise_bt);
            KeyValPair* key_pair = pair->value;
            printf("%s %f\n",(char*)key_pair->value,*((float*)key_pair->value)); 
        }

        if ( strcmp("SORTED",command) == 0)
        {
            Vector* v = binary_tree_inorder_traversal_recursive(enterprise_bt);

            for ( int i = 0; i < vector_size(v); i++)
            {
                KeyValPair* pair = v->data[i];
                KeyValPair* key_pair = pair->value;
                printf("%s %f\n",(char*)key_pair->value,*((float*)key_pair->value)); 
                key_val_pair_destroy(v->data[i]); 
            }

            vector_destroy(v);
        }
        
    }

    hash_table_destroy(enterprise_ht,enterprise_destroy);
    binary_tree_destroy(enterprise_bt);

    return 0;
}
