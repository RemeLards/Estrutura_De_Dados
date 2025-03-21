
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "vector.h"
#include "binary_tree.h"


#define MAX_ENTERPRISE_NAME 100
#define MAX_ENTERPRISE_NICKNAME 40

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



void enterprise_destroy_hashtable_item(void* data)
{
    HashTableItem* ht = data;
    if ( ht )
    {
        Enterprise* enterprise = ht->val;

        if ( enterprise != NULL )
        {
            if (enterprise->name) free(enterprise->name); 
            else printf("enterprise->name was NULL\n");
            if (enterprise->nickname) free(enterprise->nickname);
            else printf("enterprise->nickname was NULL\n");
        }
            if ( enterprise ) free(enterprise); //The rest of the variables arent pointers
        else printf("enterprise was NULL\n"); 
    }

    if ( ht ) free(ht); // Key was a pointer to enterprise->nickname, which is already freed by this time
    else printf("HashItem was NULL\n"); 

    // printf("HashItem Completed Destroyed!\n");
}



void enterprise_destroy_binary_tree_item(void* data)
{
    KeyValPair* pair = data;
    if ( pair->key ) free(pair->key);//Binary Tree will only destroy the internal pair representation and its key (which is another pair key-value)
    if ( pair ) free(pair); 
}



void* enterprise_value_construct_binary_tree(void* value)
{
    return value;
}



KeyValPair* enterprise_key_construct_binary_tree(void* key)
{
    KeyValPair* key_pair = malloc(sizeof(KeyValPair));
    Enterprise* e = key;
    key_pair->key = &(e->unit_price);
    key_pair->value = e->nickname;
    return key_pair;
}



void* enterprise_copy_key_binary_tree(void* key)
{
    KeyValPair* key_copy = key;
    if ( key )
    {
        KeyValPair* key_pair = malloc(sizeof(KeyValPair));
        key_pair->key = key_copy->key;
        key_pair->value = key_copy->value;
        return key_pair;
    }
    return key;
}



void print_enterprise(Enterprise* enterprise)
{
    // Enterprise* enterprise = pair->value;
    if (enterprise != NULL ) printf("%s %s %.2f %d %d\n",enterprise->name, enterprise->nickname, enterprise->unit_price,
                              enterprise->n_action,enterprise->n_sold);
    
    else printf("Empresa não existe!\n");
}



void read_file(FILE* f,HashTable* enterprise_ht,BinaryTree* enterprise_bt,int* n_line)
{

    if ( f != NULL )
    {
        char line[200];
        fgets(line, sizeof(line), f);
        sscanf(line, "%d",n_line); //Reads Number of Enterprises


        while (fgets(line, sizeof(line), f)) //Reads all Enterprises
        {
            Enterprise* enterprise = enterprise_construct();
            sscanf(line, "%s %s %f %d %d", enterprise->name, enterprise->nickname, &enterprise->unit_price,
                                           &enterprise->n_action, &enterprise->n_sold);
            
            hash_table_set(enterprise_ht,enterprise->nickname,enterprise);
            binary_tree_add(enterprise_bt,enterprise_key_construct_binary_tree(enterprise),enterprise_value_construct_binary_tree(enterprise));
            n_line--;
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
    int base = 31;

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
    float* unit_price_e1 = e1->key;
    float* unit_price_e2 = e2->key;

    if ( *(unit_price_e1) - *(unit_price_e2)  > 0 ) return 1 ;
    else if ( *(unit_price_e1) - *(unit_price_e2)  < 0 ) return -1 ;

    return strcmp(e1->value,e2->value);
}



int cmp_fn_interval(void *value, void *key)
{
    KeyValPair* pair = value;
    KeyValPair* keypair = pair->key;
    float* unit_price_e = keypair->key;
    float* keyval = key;

    // printf("unit price beeing compared : %f\n",*(unit_price_e));
    // printf("keyval beeing compared : %f\n",*(keyval));

    if (*(keyval) - *(unit_price_e) > 0 ) return 1 ;
    else if (*(keyval) - *(unit_price_e)  < 0 ) return -1 ;

    return 0;
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
    int n_enterprises = 0;
    int n_instructions = 0;
    char filename[65];
    scanf("%s", filename);
    FILE* f = fopen(filename,"r");
    HashTable *enterprise_ht = hash_table_construct(23, hash_str, cmp_str);
    BinaryTree *enterprise_bt = binary_tree_construct(cmp_fn, key_destroy_fn, val_destroy_fn);
    read_file(f,enterprise_ht,enterprise_bt,&n_enterprises);

    scanf("%d",&n_instructions);

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
                pair->key = &(enterprise->unit_price);
                pair->value = enterprise->nickname;


                binary_tree_remove(enterprise_bt,pair,enterprise_destroy_binary_tree_item,enterprise_copy_key_binary_tree,enterprise_value_construct_binary_tree);
                enterprise->unit_price = unit_price;
                binary_tree_add(enterprise_bt,enterprise_key_construct_binary_tree(enterprise),enterprise_value_construct_binary_tree(enterprise));

                free(pair);
            }

            free(sigla);
        }

        if ( strcmp("GET",command) == 0) //Funcionando sem individualmente
        {
            char* sigla = malloc(sizeof(char)*MAX_ENTERPRISE_NICKNAME);
            scanf("%s", sigla);

            Enterprise* enterprise = hash_table_get(enterprise_ht,sigla);
            print_enterprise(enterprise);

            free(sigla);

        }

        if ( strcmp("RM",command) == 0) //Hash funcionando
        {
            char* sigla = malloc(sizeof(char)*MAX_ENTERPRISE_NICKNAME);
            scanf("%s", sigla);
            

            Enterprise* enterprise = hash_table_get(enterprise_ht,sigla);

            KeyValPair* pair = malloc(sizeof(KeyValPair));
            pair->key = &(enterprise->unit_price);
            pair->value = enterprise->nickname;

            binary_tree_remove(enterprise_bt,pair,enterprise_destroy_binary_tree_item,enterprise_copy_key_binary_tree,enterprise_value_construct_binary_tree);
            free(hash_table_pop(enterprise_ht,enterprise->nickname,enterprise_destroy_hashtable_item));

            free(pair);
            free(sigla);
        }

        if ( strcmp("INTERVAL",command) == 0)
        {
            float* max = malloc(sizeof(float));
            float* min = malloc(sizeof(float));
            scanf("%f %f", min,max);

            Vector* v = binary_tree_interval(enterprise_bt,min,max,cmp_fn_interval);

            if ( v != NULL )
            {
                for ( int i = 0; i < vector_size(v); i++ )
                {
                    KeyValPair* pair = vector_get(v,i);
                    KeyValPair* key_pair = pair->key;
                    char* nickname = key_pair->value;
                    printf("%s\n",nickname);    
                }
                vector_destroy(v);
            }

            free(max);
            free(min);
        }


        if ( strcmp("MIN",command) == 0) //Funcionando
        {
            KeyValPair* pair = binary_tree_min(enterprise_bt);
            KeyValPair* key_pair = pair->key;
            char* nickname = key_pair->value;

            printf("%s\n",nickname); 
        }
        

        if ( strcmp("MAX",command) == 0) //Funcionando
        {
            KeyValPair* pair = binary_tree_max(enterprise_bt);
            KeyValPair* key_pair = pair->key;
            char* nickname = key_pair->value;

            printf("%s\n",nickname); 
        }


        if ( strcmp("SORTED",command) == 0) //Funcionando
        {
            Vector* v = binary_tree_inorder_traversal_recursive(enterprise_bt);

            if ( v != NULL )
            {
                for ( int i = 0; i < vector_size(v); i++)
                {
                    KeyValPair* pair = vector_get(v,i);
                    KeyValPair* key_pair = pair->key;
                    float* unit_price = key_pair->key;
                    char* nickname = key_pair->value;
                    printf("%s %.2f\n",nickname,*(unit_price)); 
                }

                vector_destroy(v);
            }
        }
        
    }
    hash_table_destroy(enterprise_ht,enterprise_destroy_hashtable_item);
    binary_tree_destroy(enterprise_bt,enterprise_destroy_binary_tree_item);

    return 0;
}
