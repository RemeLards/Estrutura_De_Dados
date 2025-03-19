#include "binary_tree.h"
#include "node.h"
#include <stdlib.h>
#include <stdio.h>


typedef struct BinaryTree
{
    Node* root;
    CmpFn cmpfn;
    KeyDestroyFn key_destroy_fn;
    ValDestroyFn val_destroy_fn;
}BinaryTree;


BinaryTree *binary_tree_construct(CmpFn cmp_fn, KeyDestroyFn key_destroy_fn,ValDestroyFn val_destroy_fn)
{
    BinaryTree* bintree = (BinaryTree*) malloc (sizeof(BinaryTree));
    bintree->root = NULL;
    bintree->cmpfn = cmp_fn;
    bintree->key_destroy_fn = key_destroy_fn;
    bintree->val_destroy_fn = val_destroy_fn;

    return bintree;
}


KeyValPair *key_val_pair_construct(void *key, void *val)
{
    KeyValPair* pair = (KeyValPair*) malloc (sizeof(KeyValPair));
    pair->key = key;
    pair->value = val;

    return pair;  
}



Node* _binary_tree_add_recursive(BinaryTree* bt, Node* root, void* key, void* value)
{
    if ( bt != NULL && key != NULL )
    {
        if ( root != NULL )
        {
            KeyValPair* temp_pair = root->value;
            if ( bt->cmpfn(temp_pair->key,key) > 0 )
            {
                if ( root->left != NULL ) _binary_tree_add_recursive(bt,root->left,key,value);
                else
                {
                    KeyValPair* pair = key_val_pair_construct(key,value);
                    root->left = node_construct(pair,NULL,NULL);
                } 
            } 
            else if ( bt->cmpfn(temp_pair->key,key) < 0 )
            {
                if ( root->right != NULL )_binary_tree_add_recursive(bt,root->right,key,value);
                else
                {
                    KeyValPair* pair = key_val_pair_construct(key,value);
                    root->right = node_construct(pair,NULL,NULL);
                } 
            }
        }

        else
        {
            KeyValPair* pair = key_val_pair_construct(key,value);
            bt->root = node_construct(pair,NULL,NULL);
        }
    }

    return NULL;
}



void binary_tree_add(BinaryTree *bt, void *key, void *value)
{
    if ( bt != NULL )
    {
        _binary_tree_add_recursive(bt,bt->root,key,value);
    }
}



void* binary_tree_get(BinaryTree *bt, void *key)
{
    if ( bt != NULL )
    {
        Node* root_copy = bt->root;

        while ( root_copy != NULL )
        {
            KeyValPair* temp_pair = root_copy->value;

            if ( bt->cmpfn(key,temp_pair->key) == 0 )
            {
                return (void*) temp_pair->value;
            }

            if ( bt->cmpfn(key,temp_pair->key) < 0 )
            {
                if ( root_copy->left != NULL )root_copy = root_copy->left;
                else break;
            }
            else if ( bt->cmpfn(key,temp_pair->key) > 0 )
            {
                if ( root_copy->right != NULL )root_copy = root_copy->right;
                else break;
            }
        }
    }

    return NULL;
}



void key_val_pair_destroy(KeyValPair *kvp)
{
    free(kvp->key);
    free(kvp->value);
    free(kvp);
}



void _binary_tree_destroy_recursive(Node* root)
{
    if ( root != NULL )
    {
        if ( root->left != NULL ) 
        {
            _binary_tree_destroy_recursive(root->left);
        }
        if (root->right != NULL ) 
        {
            _binary_tree_destroy_recursive(root->right);
        }
        key_val_pair_destroy(root->value);
        node_destroy(root,NULL);
    }
} 



void binary_tree_destroy(BinaryTree *bt)
{
    if ( bt != NULL )
    {
        _binary_tree_destroy_recursive(bt->root);
        free(bt);
    }
}



void _binary_tree_print_recursive(Node* root)
{
    if ( root != NULL )
    {
        KeyValPair* temp_pair = root->value;
        printf("(%d",*((int*)temp_pair->key));

        if ( root->left != NULL )
        {
            printf(", ");
            _binary_tree_print_recursive(root->left);
        }
        else 
        {
            printf(", NULL");
        }

        if ( root->right != NULL )
        {
            printf(", ");
            _binary_tree_print_recursive(root->right);
        }
        else 
        {
            printf(", NULL");
        }
        printf(")");
    }
}



void binary_tree_print(BinaryTree *bt)
{
    if ( bt != NULL )
    {
        _binary_tree_print_recursive(bt->root);
    }
}



Node* _binary_tree_min_right_subtree_parent(Node* root)
{
    if ( root != NULL )
    {
        Node* parent = root;
        Node* right_root = root->right;

        if ( right_root != NULL )
        {
            while ( right_root->left != NULL )
            {
                parent = right_root;
                right_root = right_root->left;
            }
        }
        // KeyValPair* debug_pair = parent->value;
        // printf("Choosen parent key : %d\n",*((int*)debug_pair->key));

        return parent;

    }

    return NULL;
}

// Function to remove a node
void _binary_tree_remove(BinaryTree* bt, Node* root, void* key, Node* parent) 
{    
    if (root != NULL)
    {
        KeyValPair* temp_pair = root->value;
        // Search for the node
        if (bt->cmpfn(temp_pair->key,key) > 0) 
        {
            _binary_tree_remove(bt,root->left,key,root);
        } 
        else if (bt->cmpfn(temp_pair->key,key) < 0) 
        {
            _binary_tree_remove(bt,root->right,key,root);
        } 
        else 
        {
            if (root->left == NULL && root->right == NULL)
            {
                key_val_pair_destroy(root->value);
                node_destroy(root,NULL);
                if ( parent == NULL ) bt->root = NULL;
                else if ( parent->left == root) parent->left = NULL;
                else parent->right = NULL;
            }
            else if (root->left == NULL)
            {
                if ( parent == NULL )bt->root = root->right;
                else if ( parent->left == root ) parent->left = root->right;
                else parent->right = root->right;
                key_val_pair_destroy(root->value);
                node_destroy(root,NULL);
            }
            else if (root->right == NULL)
            {
                if ( parent == NULL )bt->root = root->left;
                else if ( parent->left == root ) parent->left = root->left;
                else parent->right = root->left;
                key_val_pair_destroy(root->value);
                node_destroy(root,NULL);
            }
            else
            {
                // Case 2: Node with two children
                KeyValPair* temp_pair;
                Node* temp = _binary_tree_min_right_subtree_parent(root);

                if ( temp == bt->root ) // Root
                {
                    temp_pair = temp->right->value;

                    int* key = malloc(sizeof(int));
                    int* value = malloc(sizeof(int));
                    *key = *((int*)temp_pair->key);
                    *value = *((int*)temp_pair->value);
                    temp_pair = key_val_pair_construct(key,value);
                    _binary_tree_remove(bt,temp->right, temp_pair->key,temp);
                }
                else
                {
                    temp_pair = temp->left->value;

                    int* key = malloc(sizeof(int));
                    int* value = malloc(sizeof(int));
                    *key = *((int*)temp_pair->key);
                    *value = *((int*)temp_pair->value);
                    temp_pair = key_val_pair_construct(key,value);
                    // printf("Choosen key recursion : %d\n",*key);
                    _binary_tree_remove(bt,temp->left, temp_pair->key,temp);
                }
                _binary_tree_remove(bt,temp->right, temp_pair->key,temp);
                key_val_pair_destroy(root->value);
                root->value = temp_pair;
            }
        }
    }
}

void _binary_tree_remove_2(BinaryTree* bt, Node* root, void *key)
{
    if ( root != NULL )
    {
        printf("passei aqui 1\n");
        Node* root_copy = root;
        Node* parent = NULL;
        while ( !(root_copy->left == NULL && root_copy->right == NULL ) )
        {
            printf("passei aqui 8\n");
            KeyValPair* temp_pair = root_copy->value;
            if ( bt->cmpfn(temp_pair->key,key) == 0 ) break; //Root key == key
            printf("temp pair key : %d & key : %d\n",*((int*)temp_pair->key),*((int*)key));

            printf("passei aqui 9\n");
            parent = root_copy;
            if ( bt->cmpfn(temp_pair->key,key) > 0 ) root_copy = root_copy->left; // Root key > key
            else root_copy = root_copy->right; // Root key < key
            printf("passei aqui 10\n");

        }
        printf("passei aqui 2\n");

        if ( root_copy != NULL )
        {
            // Caso não tenha filhos
            if ( root_copy->left == NULL && root_copy->right == NULL )
            {
                printf("passei aqui 3\n");
                key_val_pair_destroy(root_copy->value);
                node_destroy(root_copy,NULL);
                if ( parent == NULL )bt->root = NULL;
                else if ( parent->left == root_copy)parent->left = NULL;
                else parent->right = NULL; 
            }

            // Caso tenha um filho

            else if ( root_copy->left != NULL && root_copy->right == NULL )
            {
                printf("passei aqui 4\n");
                Node* temp_node = root_copy;
                if ( parent == NULL )
                {
                    bt->root = root_copy->left;
                    key_val_pair_destroy(temp_node->value);
                    node_destroy(temp_node,NULL);
                }
                else if ( parent->left == root_copy )
                {
                    parent->left = root_copy->left;
                    key_val_pair_destroy(temp_node->value);
                    node_destroy(temp_node,NULL);
                }
                else
                {
                    parent->right = root_copy->left;
                    key_val_pair_destroy(temp_node->value);
                    node_destroy(temp_node,NULL);    
                }
            }

            else if ( root_copy->left == NULL && root_copy->right != NULL )
            {
                printf("passei aqui 5\n");
                Node* temp_node = root_copy;
                if ( parent == NULL )
                {
                    bt->root = root_copy->right;
                    key_val_pair_destroy(temp_node->value);
                    node_destroy(temp_node,NULL);
                }
                else if ( parent->left == root_copy )
                { 
                    parent->left = root_copy->right;
                    key_val_pair_destroy(temp_node->value);
                    node_destroy(temp_node,NULL);
                }
                else
                {
                    parent->right = root_copy->right;
                    key_val_pair_destroy(temp_node->value);
                    node_destroy(temp_node,NULL); 
                }
            }

            // Caso tenha dois filhos 

            else
            {
                printf("passei aqui 6\n");
                KeyValPair* temp_pair;
                Node* min_parent_right_subtree = _binary_tree_min_right_subtree_parent(root_copy);

                if ( min_parent_right_subtree == bt->root ) // Root
                {
                    temp_pair = min_parent_right_subtree->right->value;

                    int* key = malloc(sizeof(int));
                    int* value = malloc(sizeof(int));
                    *key = *((int*)temp_pair->key);
                    *value = *((int*)temp_pair->value);
                    temp_pair = key_val_pair_construct(key,value);
                }
                else
                {
                    temp_pair = min_parent_right_subtree->left->value;

                    int* key = malloc(sizeof(int));
                    int* value = malloc(sizeof(int));
                    *key = *((int*)temp_pair->key);
                    *value = *((int*)temp_pair->value);
                    temp_pair = key_val_pair_construct(key,value);
                    // printf("Choosen key recursion : %d\n",*key);
                }
                _binary_tree_remove_2(bt,min_parent_right_subtree,temp_pair->key);
                key_val_pair_destroy(root_copy->value);
                root_copy->value = temp_pair;
                printf("passei aqui 7\n");

            }
        }
    }
}


void binary_tree_remove(BinaryTree *bt, void *key)
{
    if ( bt != NULL )
    { 
        _binary_tree_remove_2(bt,bt->root,key);
    }
}



KeyValPair *binary_tree_pop_min(BinaryTree *bt)
{
    if (bt != NULL)
    {
        Node* root_copy = bt->root;
        Node* parent = NULL;

        while ( root_copy->left != NULL )
        {
            parent = root_copy;
            root_copy = root_copy->left;
        }


        KeyValPair* pair = NULL;
        if ( root_copy != bt->root)
        {
            if ( root_copy->right == NULL )parent->left = NULL;
            else parent->left = root_copy->right;
            pair  = root_copy->value;
            node_destroy(root_copy,NULL);
        }
        else
        {
            bt->root = root_copy->right;
            pair  = root_copy->value;
            node_destroy(root_copy,NULL);
        }


        return pair;

    }

    return NULL;
}



KeyValPair *binary_tree_pop_max(BinaryTree *bt)
{
    if (bt != NULL)
    {
        Node* root_copy = bt->root;
        Node* parent = NULL;

        while ( root_copy->right != NULL )
        {
            parent = root_copy;
            root_copy = root_copy->right;
        }

        KeyValPair* pair = NULL;
        if ( root_copy != bt->root )
        {
            if (root_copy->left == NULL) parent->right = NULL;
            else  parent->right = root_copy->left;
            pair  = root_copy->value;
            node_destroy(root_copy,NULL);
        }
        else
        {
            bt->root = root_copy->left;
            pair  = root_copy->value;
            node_destroy(root_copy,NULL);
        }
        return pair;
    }
    return NULL;
}