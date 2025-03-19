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



Node* _binary_tree_add_recursive(BinaryTree* bt, Node* node, void* key, void* value)
{
    if ( bt != NULL && key != NULL )
    {
        if ( node == NULL )
        {
            KeyValPair* pair = key_val_pair_construct(key,value);
            return node_construct(pair,NULL,NULL);
        }
        
        else
        {
            KeyValPair* temp_pair = node->value;

            if ( bt->cmpfn(temp_pair->key,key) > 0 )
            {
                node->left = _binary_tree_add_recursive(bt,node->left,key,value); 
            } 
            else if ( bt->cmpfn(temp_pair->key,key) < 0 )
            {
                node->right = _binary_tree_add_recursive(bt,node->right,key,value);
            }

            return node;
        }
    }

    return NULL;
}



void binary_tree_add(BinaryTree *bt, void *key, void *value)
{
    if ( bt != NULL )
    {
        bt->root = _binary_tree_add_recursive(bt,bt->root,key,value);
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



Node* _binary_tree_min_right_subtree(Node* root)
{
    if ( root != NULL )
    {
        Node* right_root = root->right;

        if ( right_root != NULL )
        {
            while ( right_root->left != NULL )
            {
                right_root = right_root->left;
            }
        }
        // KeyValPair* debug_pair = parent->value;
        // printf("Choosen parent key : %d\n",*((int*)debug_pair->key));

        return right_root;

    }

    return NULL;
}




Node* _binary_tree_remove(BinaryTree* bt, Node* root, void *key)
{
    if ( root != NULL )
    {
    
        KeyValPair* current_pair;
        Node* current_node = root;
        Node* parent = NULL;

        if ( current_node != NULL ) current_pair = current_node->value;
        while ( current_node != NULL && bt->cmpfn(current_pair->key,key) != 0)
        {
            printf("passei aqui loop\n");
            parent = current_node;
            if ( bt->cmpfn(current_pair->key,key) > 0 ) current_node = current_node->left; // Root key > key
            else current_node = current_node->right; // Root key < key
            current_pair = current_node->value;

        }
        printf("passei aqui 1\n");
        printf("key node choosen : %d\n", *((int*)current_pair->key));
        if( current_node->right == NULL ) printf("right NULL\n");
        if( current_node->left == NULL ) printf("left NULL\n");

        if ( current_node != NULL )
        {
            // Caso não tenha filhos
            if ( current_node->left == NULL && current_node->right == NULL )
            {
                printf("passei aqui 1\n");
                if ( parent == NULL ) bt->root = NULL;
                else if ( parent->left == current_node) parent->left = NULL;
                else parent->right = NULL; 
            }

            // Caso tenha um filho

            else if ( current_node->right == NULL )
            {
                printf("passei aqui 2\n");
                if ( parent == NULL ) bt->root = current_node->left;
                else if ( parent->left == root ) parent->left = current_node->left;
                else parent->right = current_node->left;
            }

            else if ( current_node->left == NULL )
            {
                printf("passei aqui 3\n");
                if ( parent == NULL ) bt->root = current_node->right;
                else if ( parent->left == root ) parent->left = current_node->right;
                else parent->right = current_node->right;
            }

            // Caso tenha dois filhos 

            else
            {
                printf("passei aqui 4\n");
                KeyValPair*  min_pair;
                Node* min_parent_right_subtree = _binary_tree_min_right_subtree(current_node);
                min_pair = min_parent_right_subtree->value;
                
                Node* node_to_remove = _binary_tree_remove(bt,root,min_pair->key);
                key_val_pair_destroy(current_node->value);
                current_node->value = node_to_remove->value;
                node_to_remove->value = NULL;
                current_node=node_to_remove;
            }

            return current_node;
        }
    }

    return NULL;
}



void binary_tree_remove(BinaryTree *bt, void *key)
{
    if ( bt != NULL )
    { 
        Node* temp = _binary_tree_remove(bt,bt->root,key);

        if ( temp-> value != NULL ) key_val_pair_destroy(temp->value);
        node_destroy(temp,NULL);
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