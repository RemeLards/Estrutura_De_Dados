
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "vector.h"
#include "binary_tree.h"
#include <math.h>


#define MAX_PLAYER_NAME 100
#define MAX_PLAYER_NICKNAME 40

typedef struct Player
{
    char*   name;
    char*   nickname;
    int     matches;
    int     victories;
}Player;


Player* player_construct()
{
    Player* player = malloc(sizeof(Player));
    player->name = malloc(MAX_PLAYER_NAME*sizeof(char));;
    player->nickname = malloc(MAX_PLAYER_NICKNAME*sizeof(char));
    player->matches = 0;
    player->victories = 0;
    return player;
}



void player_destroy_hashtable_item(void* data)
{
    HashTableItem* ht = data;
    if ( ht )
    {
        Player* player = ht->val;

        if ( player != NULL )
        {
            if (player->name) free(player->name); 
            else printf("player->name was NULL\n");
            if (player->nickname) free(player->nickname);
            else printf("player->nickname was NULL\n");
        }
            if ( player ) free(player); //The rest of the variables arent pointers
        else printf("player was NULL\n"); 
    }

    if ( ht ) free(ht); // Key was a pointer to player->nickname, which is already freed by this time
    else printf("HashItem was NULL\n"); 

    // printf("HashItem Completed Destroyed!\n");
}



void player_destroy_binary_tree_item(void* data)
{
    KeyValPair* pair = data;
    KeyValPair* keypair = pair->key;
    if ( keypair->key ) free(keypair->key); //Now I malloc the player WinRate
    if ( pair->key ) free(pair->key);//Binary Tree will only destroy the internal pair representation and its key (which is another pair key-value)
    if ( pair ) free(pair); 
}



void* player_value_construct_binary_tree(void* value)
{
    return value;
}



KeyValPair* player_key_construct_binary_tree(void* key)
{
    KeyValPair* key_pair = malloc(sizeof(KeyValPair));
    Player* player = key;
    float* winrate = malloc(sizeof(float));
    *(winrate) = player->victories/(player->matches); 
    key_pair->key = winrate; //Agora vou ter que destruir a chave do par de chaves
    key_pair->value = player->nickname;
    return key_pair;
}



void* player_copy_key_binary_tree(void* key)
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



void print_player(Player* player)
{
    // Player* player = pair->value;
    if (player != NULL ) printf("%s %s %d %d\n",player->nickname, player->name, player->victories,
                              player->matches);
    
    else printf("Player não existe!\n");
}



void read_file(FILE* f,HashTable* player_ht,BinaryTree* player_bt,int* n_line)
{

    if ( f != NULL )
    {
        char line[200];
        fgets(line, sizeof(line), f);
        sscanf(line, "%d",n_line); //Reads Number of Enterprises


        while (fgets(line, sizeof(line), f)) //Reads all Enterprises
        {
            Player* player = player_construct();
            sscanf(line, "%s %s %d %d", player->nickname, player->name, &player->victories,
                                           &player->matches);
            
            hash_table_set(player_ht,player->nickname,player);
            binary_tree_add(player_bt,player_key_construct_binary_tree(player),player_value_construct_binary_tree(player));
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
    KeyValPair* p1 = key1;
    KeyValPair* p2 = key2;
    float* wr_p1 = p1->key;
    float* wr_p2 = p2->key;

    if ( *(wr_p1) - *(wr_p2)  > 0 ) return 1 ;
    else if ( *(wr_p1) - *(wr_p2)  < 0 ) return -1 ;

    return strcmp(p1->value,p2->value);
}



int cmp_fn_interval(void *value, void *key)
{
    KeyValPair* pair = value;
    KeyValPair* keypair = pair->key;
    float* wr = keypair->key;
    float* keyval = key;

    // printf("unit price beeing compared : %f\n",*(unit_price_e));
    // printf("keyval beeing compared : %f\n",*(keyval));

    if (*(keyval) - *(wr) > 0 ) return 1 ;
    else if (*(keyval) - *(wr)  < 0 ) return -1 ;

    return 0;
}



void cmp_knn(void* key1, void* key2, void* key3)
{
    KeyValPair* keypair2 = key2;
    float* player2_wr = keypair2->key;
    KeyValPair* keypair3 = key3;
    float* player3_wr = keypair3->key;

    if ( key1 == NULL )
    {
        key1 = keypair3;
    }
    else
    {
        KeyValPair* keypair1 = key1;
        float* player1_wr = keypair1->key;

        if ( fabs(*player1_wr-*player2_wr) > fabs(*player3_wr-*player2_wr)  ) key1 = keypair3; 
    }
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
    int n_players = 0;
    int n_instructions = 0;
    char filename[65];
    scanf("%s", filename);
    FILE* f = fopen(filename,"r");
    HashTable *player_ht = hash_table_construct(23, hash_str, cmp_str);
    BinaryTree *player_bt = binary_tree_construct(cmp_fn, key_destroy_fn, val_destroy_fn);
    read_file(f,player_ht,player_bt,&n_players);

    scanf("%d",&n_instructions);

    for ( int i = 0; i < n_instructions; i++)
    {
        char command[65];

        scanf("%s", command);

        if ( strcmp("VICTORIES",command) == 0)
        {
            char* sigla = malloc(sizeof(char)*MAX_PLAYER_NICKNAME);
            float victories = 0;
            scanf("%s %f", sigla,&victories);

            //Hashtable Part
            Player* player = hash_table_get(player_ht,sigla);
            if ( player != NULL )
            {
                //Binary Tree Part

                KeyValPair* pair = malloc(sizeof(KeyValPair));
                float* wr = malloc(sizeof(float));
                *wr = player->victories/player->matches;
                pair->key = wr;
                pair->value = player->nickname;


                binary_tree_remove(player_bt,pair,player_destroy_binary_tree_item,player_copy_key_binary_tree,player_value_construct_binary_tree);
                player->victories +=victories;
                player->matches +=victories;
                binary_tree_add(player_bt,player_key_construct_binary_tree(player),player_value_construct_binary_tree(player));

                free(wr);
                free(pair);
            }

            free(sigla);
        }

        if ( strcmp("DEFEATS",command) == 0)
        {
            char* sigla = malloc(sizeof(char)*MAX_PLAYER_NICKNAME);
            float defeats = 0;
            scanf("%s %f", sigla,&defeats);

            //Hashtable Part
            Player* player = hash_table_get(player_ht,sigla);
            if ( player != NULL )
            {
                //Binary Tree Part

                KeyValPair* pair = malloc(sizeof(KeyValPair));
                float* wr = malloc(sizeof(float));
                *wr = player->victories/player->matches;
                pair->key = wr;
                pair->value = player->nickname;

                binary_tree_remove(player_bt,pair,player_destroy_binary_tree_item,player_copy_key_binary_tree,player_value_construct_binary_tree);
                player->matches +=defeats;
                binary_tree_add(player_bt,player_key_construct_binary_tree(player),player_value_construct_binary_tree(player));

                free(wr);
                free(pair);
            }

            free(sigla);
        }

        if ( strcmp("GET",command) == 0) //Funcionando sem individualmente
        {
            char* sigla = malloc(sizeof(char)*MAX_PLAYER_NICKNAME);
            scanf("%s", sigla);

            Player* player = hash_table_get(player_ht,sigla);
            print_player(player);

            free(sigla);

        }

        if ( strcmp("RM",command) == 0) //Hash funcionando
        {
            char* sigla = malloc(sizeof(char)*MAX_PLAYER_NICKNAME);
            scanf("%s", sigla);
            

            Player* player = hash_table_get(player_ht,sigla);

            KeyValPair* pair = malloc(sizeof(KeyValPair));
            float* wr = malloc(sizeof(float));
            *wr = player->victories/player->matches;
            pair->key = wr;
            pair->value = player->nickname;

            binary_tree_remove(player_bt,pair,player_destroy_binary_tree_item,player_copy_key_binary_tree,player_value_construct_binary_tree);
            hash_table_remove(player_ht,player->nickname,player_destroy_hashtable_item);

            free(wr);
            free(pair);
            free(sigla);
        }

        if ( strcmp("INTERVAL",command) == 0)
        {
            float* max = malloc(sizeof(float));
            float* min = malloc(sizeof(float));
            scanf("%f %f", min,max);

            Vector* v = binary_tree_interval(player_bt,min,max,cmp_fn_interval);

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
            KeyValPair* pair = binary_tree_min(player_bt);
            KeyValPair* key_pair = pair->key;
            char* nickname = key_pair->value;

            printf("%s\n",nickname); 
        }
        
        if ( strcmp("MAX",command) == 0) //Funcionando
        {
            KeyValPair* pair = binary_tree_max(player_bt);
            KeyValPair* key_pair = pair->key;
            char* nickname = key_pair->value;

            printf("%s\n",nickname); 
        }

        if ( strcmp("SORTED",command) == 0) //Funcionando
        {
            Vector* v = binary_tree_reverse_inorder_traversal_recursive(player_bt);

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

        if ( strcmp("MATCH",command) == 0) //Funcionando
        {
            KeyValPair* nearest_player = binary_tree_knn(player_bt,NULL,cmp_knn);
            Player* player = hash_table_get(player_ht,nearest_player->value);
            printf("%s\n",player->nickname);

        }


    }
    hash_table_destroy(player_ht,player_destroy_hashtable_item);
    binary_tree_destroy(player_bt,player_destroy_binary_tree_item);

    return 0;
}
