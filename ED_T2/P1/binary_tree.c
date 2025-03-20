#include "binary_tree.h"
#include "node_tree.h"
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
                    root->left = node_construct_tree(pair,NULL,NULL);
                } 
            } 
            else if ( bt->cmpfn(temp_pair->key,key) < 0 )
            {
                if ( root->right != NULL )_binary_tree_add_recursive(bt,root->right,key,value);
                else
                {
                    KeyValPair* pair = key_val_pair_construct(key,value);
                    root->right = node_construct_tree(pair,NULL,NULL);
                } 
            }

            else if ( bt -> cmpfn(temp_pair->key,key) == 0)
            {
                bt->key_destroy_fn(key);
                bt->val_destroy_fn(value);
            }
        }

        else
        {
            KeyValPair* pair = key_val_pair_construct(key,value);
            bt->root = node_construct_tree(pair,NULL,NULL);
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
    free(kvp->key); // Changed so Enterprise doesnt get freed
    free(kvp->value); // Changed so Enterprise doesnt get freed
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
        if ( root->value != NULL ) key_val_pair_destroy(root->value);
        node_destroy_tree(root,NULL);
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

        return right_root;

    }

    return NULL;
}



void _binary_tree_remove(BinaryTree* bt, Node* root, void *key)
{
    if ( root != NULL )
    {
        Node* root_copy = root;
        Node* parent = NULL;
        while ( !(root_copy->left == NULL && root_copy->right == NULL ) )
        {
            KeyValPair* temp_pair = root_copy->value;
            if ( bt->cmpfn(temp_pair->key,key) == 0 ) break; //Root key == key

            parent = root_copy;
            if ( bt->cmpfn(temp_pair->key,key) > 0 ) root_copy = root_copy->left; // Root key > key
            else root_copy = root_copy->right; // Root key < key

        }

        if ( root_copy != NULL )
        {
            // Caso não tenha filhos
            if ( root_copy->left == NULL && root_copy->right == NULL )
            {
                key_val_pair_destroy(root_copy->value);
                node_destroy_tree(root_copy,NULL);
                if ( parent == NULL )bt->root = NULL;
                else if ( parent->left == root_copy)parent->left = NULL;
                else parent->right = NULL; 
            }

            // Caso tenha um filho

            else if ( root_copy->left != NULL && root_copy->right == NULL )
            {
                Node* temp_node = root_copy;
                if ( parent == NULL )
                {
                    bt->root = root_copy->left;
                    key_val_pair_destroy(temp_node->value);
                    node_destroy_tree(temp_node,NULL);
                }
                else if ( parent->left == root_copy )
                {
                    parent->left = root_copy->left;
                    key_val_pair_destroy(temp_node->value);
                    node_destroy_tree(temp_node,NULL);
                }
                else
                {
                    parent->right = root_copy->left;
                    key_val_pair_destroy(temp_node->value);
                    node_destroy_tree(temp_node,NULL);    
                }
            }

            else if ( root_copy->left == NULL && root_copy->right != NULL )
            {
                Node* temp_node = root_copy;
                if ( parent == NULL )
                {
                    bt->root = root_copy->right;
                    key_val_pair_destroy(temp_node->value);
                    node_destroy_tree(temp_node,NULL);
                }
                else if ( parent->left == root_copy )
                { 
                    parent->left = root_copy->right;
                    key_val_pair_destroy(temp_node->value);
                    node_destroy_tree(temp_node,NULL);
                }
                else
                {
                    parent->right = root_copy->right;
                    key_val_pair_destroy(temp_node->value);
                    node_destroy_tree(temp_node,NULL); 
                }
            }

            // Caso tenha dois filhos 

            else
            {
                KeyValPair* temp_pair;
                Node* min_parent_right_subtree = _binary_tree_min_right_subtree(root_copy);
                temp_pair = min_parent_right_subtree->value;

                int* key = malloc(sizeof(int));
                int* value = malloc(sizeof(int));
                *key = *((int*)temp_pair->key);
                *value = *((int*)temp_pair->value);
                temp_pair = key_val_pair_construct(key,value);

                _binary_tree_remove(bt,bt->root,temp_pair->key);
                key_val_pair_destroy(root_copy->value);
                root_copy->value = temp_pair;

            }
        }
    }
}



void binary_tree_remove(BinaryTree *bt, void *key)
{
    if ( bt != NULL )
    { 
        _binary_tree_remove(bt,bt->root,key);
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
            node_destroy_tree(root_copy,NULL);
        }
        else
        {
            bt->root = root_copy->right;
            pair  = root_copy->value;
            node_destroy_tree(root_copy,NULL);
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
            node_destroy_tree(root_copy,NULL);
        }
        else
        {
            bt->root = root_copy->left;
            pair  = root_copy->value;
            node_destroy_tree(root_copy,NULL);
        }
        return pair;
    }
    return NULL;
}



KeyValPair *binary_tree_min(BinaryTree *bt)
{
    if (bt != NULL)
    {
        Node* root_copy = bt->root;

        while ( root_copy->left != NULL )
        {
            root_copy = root_copy->left;
        }

        KeyValPair* pair = NULL;
        pair  = root_copy->value;


        return pair;

    }

    return NULL;
}



KeyValPair *binary_tree_max(BinaryTree *bt)
{
    if (bt != NULL)
    {
        Node* root_copy = bt->root;

        while ( root_copy->right != NULL )
        {
            root_copy = root_copy->right;
        }

        KeyValPair* pair = NULL;
        pair  = root_copy->value;
        return pair;
    }
    return NULL;
}




void _binary_tree_levelorder_traversal_iterative(BinaryTree *bt, Node* root, Vector* v)
{
    if ( bt != NULL )
    {
        if ( root != NULL )
        {
            Vector* queue = vector_construct();

            vector_push_back(queue,root);

            while ( vector_size(queue) != 0 )
            {
                Node* current = vector_pop_front(queue);
                KeyValPair* current_pair = current->value;

                int* key = malloc(sizeof(int));
                key = current_pair->key;
                int* value = malloc(sizeof(int));
                value = current_pair->value;
                KeyValPair* pair = key_val_pair_construct(key,value);

                vector_push_back(v,pair);

                if ( current->left != NULL )
                {
                    vector_push_back(queue,current->left);
                }
                if ( current->right != NULL )
                {
                    vector_push_back(queue,current->right);
                }
            }

            vector_destroy(queue);

        }
    }
}



Vector* binary_tree_levelorder_traversal(BinaryTree *bt)
{
    if ( bt != NULL )
    {
        Vector* v = vector_construct();
        _binary_tree_levelorder_traversal_iterative(bt,bt->root,v);
        return v;
    }

    return NULL;
}



void _binary_tree_inorder_traversal_recursive(BinaryTree* bt, Node* root, Vector* v)
{
    if ( bt != NULL )
    {
        if ( root != NULL )
        {
            
            if ( root->left != NULL ) _binary_tree_inorder_traversal_recursive(bt,root->left,v);


            KeyValPair* current_pair = root->value;

            int* key = malloc(sizeof(int));
            key = current_pair->key;
            int* value = malloc(sizeof(int));
            value = current_pair->value;
            KeyValPair* pair = key_val_pair_construct(key,value);

            vector_push_back(v,pair);

            if ( root->right != NULL ) _binary_tree_inorder_traversal_recursive(bt,root->right,v);

        }
    }
}



Vector* binary_tree_inorder_traversal_recursive(BinaryTree *bt)
{
    if ( bt != NULL )
    {
        Vector* v = vector_construct();
        _binary_tree_inorder_traversal_recursive(bt,bt->root,v);
        return v;
    }

    return NULL;
}


void _binary_tree_preorder_traversal_recursive(BinaryTree* bt, Node* root, Vector* v)
{
    if ( bt != NULL )
    {
        if ( root != NULL )
        {
            KeyValPair* current_pair = root->value;

            int* key = malloc(sizeof(int));
            key = current_pair->key;
            int* value = malloc(sizeof(int));
            value =current_pair->value;
            KeyValPair* pair = key_val_pair_construct(key,value);

            vector_push_back(v,pair);
            
            if ( root->left != NULL ) _binary_tree_preorder_traversal_recursive(bt,root->left,v);
            if ( root->right != NULL ) _binary_tree_preorder_traversal_recursive(bt,root->right,v);

        }
    }
}



Vector* binary_tree_preorder_traversal_recursive(BinaryTree *bt)
{
    if ( bt != NULL )
    {

        Vector* v = vector_construct();
        _binary_tree_preorder_traversal_recursive(bt,bt->root,v);
        return v;
    }

    return NULL;
}


void _binary_tree_postorder_traversal_recursive(BinaryTree* bt, Node* root, Vector* v)
{
    if ( bt != NULL )
    {
        if ( root != NULL )
        {          
            if ( root->left != NULL ) _binary_tree_postorder_traversal_recursive(bt,root->left,v);
            if ( root->right != NULL ) _binary_tree_postorder_traversal_recursive(bt,root->right,v);

            KeyValPair* current_pair = root->value;

            int* key = malloc(sizeof(int));
            *key = *((int*)current_pair->key);
            int* value = malloc(sizeof(int));
            *value = *((int*)current_pair->value);
            KeyValPair* pair = key_val_pair_construct(key,value);  

            vector_push_back(v,pair);

        }
    }
}



Vector* binary_tree_postorder_traversal_recursive(BinaryTree *bt)
{
    if ( bt != NULL )
    {

        Vector* v = vector_construct();
        _binary_tree_postorder_traversal_recursive(bt,bt->root,v);
        return v;
    }

    return NULL;

}


