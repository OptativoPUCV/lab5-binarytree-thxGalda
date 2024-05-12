#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    return new;
}



void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (tree == NULL && key == NULL && value == NULL) return;
    TreeNode *aux = tree->root;
    TreeNode *parent = NULL;

    while (aux != NULL){
        parent = aux;
        if (tree->lower_than(key, aux->pair->key)) 
            aux = aux->left;
        else if (tree->lower_than(aux->pair->key, key))
            aux = aux->right;
        else
            return;
    }
    TreeNode *new = createTreeNode(key, value);
    if (tree->lower_than(new->pair->key, parent->pair->key))
        parent->left = new;
    else
        parent->right = new;
    new->parent = parent;
    tree->current = new;
}

TreeNode * minimum(TreeNode * x){
    TreeNode *aux = x;
    while (aux->left != NULL){
        aux = aux->left;
    }
    return aux;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if (tree == NULL && node == NULL) return;
    if (node->left == NULL && node->right == NULL){
        if (node->parent->right == node)
            node->parent->right = NULL;
        else
            node->parent->left = NULL;
    }
    else{
        if (node->left != NULL && node->right != NULL){
            TreeNode *aux = minimum(node->right);
            node->pair->key = aux->pair->key;
            node->pair->value = aux->pair->value;
            removeNode(tree, aux);
        }
        else{
            if (node->parent->right == node){
                node->parent->right = node->left;
                node->left->parent = node->parent;
            }
            else{
                node->parent->left = node->left;
                node->left->parent = node->parent;
            }
            
        }
    }
    
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;
    
    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode *aux = tree->root;
    while (aux != NULL){
        if (tree->lower_than(key, aux->pair->key)){
            aux = aux->left;
        }
        else{
            if (tree->lower_than(aux->pair->key, key))
                aux = aux->right;
            else
            {
                tree->current = aux;
                return aux->pair;
            }
                
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode *aux = tree->root;
    TreeNode *ub = NULL;
    while (aux != NULL){
        if (tree->lower_than(key, aux->pair->key)){
            aux = aux->left;
        }
        else{
            if (tree->lower_than(aux->pair->key, key)){
                ub = aux;
                aux = aux->right;
            }
            else{
                tree->current = aux;
                return aux->pair;
            }
        }
    }
    return ub->pair;
}

Pair * firstTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode *aux = minimum(tree->root);
    tree->current = aux;
    return aux->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->current == NULL) return NULL;
    TreeNode *aux = tree->current;
    if (aux->right != NULL){
        aux = minimum(aux->right);
        tree->current = aux;
        return aux->pair;
    }
    else{
        TreeNode *parent = aux->parent;
        while (parent != NULL && aux == parent->right){
            aux = parent;
            parent = parent->parent;
        }
        tree->current = parent;
        if (parent != NULL) return parent->pair;
    }
    return NULL;
}
