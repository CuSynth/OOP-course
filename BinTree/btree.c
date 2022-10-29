#include <stdlib.h>
#include <string.h>

#include "btree.h"

#define STOP NULL

typedef struct Node Node;

struct Node {
    Node* left;
    Node* right;
    Node* parent;
    BTreeItem item;
};

typedef struct BTree {
    Node* head;

    size_t keySize;
    size_t valueSize;
    size_t elemCount;
    int(*cmp)(const void*, const void*);
} BTree;


void* btree_init(
    void* btree,
    size_t keySize,
    size_t valueSize,
    int(*compare)(const void*, const void*),
    void(*destroy)(void*)) 
{
    if (keySize == 0 || valueSize == 0 || compare == NULL)
        return NULL;

    BTree* tree = (BTree*)btree;

    if(tree != NULL)
        btree_clear(btree, destroy);
    else {
        tree = (BTree*)malloc(sizeof(BTree));
        if (tree == NULL)
            return NULL;
    }

    tree->head = NULL;
    tree->keySize = keySize;
    tree->valueSize = valueSize;
    tree->elemCount = 0;
    tree->cmp = compare;
    return tree;
}

void* btree_create(
    size_t keySize, 
    size_t valueSize, 
    int(*compare)(const void*, const void*)) 
{
    return btree_init(NULL, keySize, valueSize, compare, NULL);
}

static void destroy_nodes(Node* startNode, void(*destroy)(void*)) {
    if (startNode == NULL)
        return;

    destroy_nodes(startNode->left, destroy);
    destroy_nodes(startNode->right, destroy);

    if(destroy != NULL)
        destroy(&(startNode->item));

    free(startNode->item.value);
    free((void*)startNode->item.key);
    free(startNode);
}

void btree_destroy(void* btree, void(*destroy)(void*)) {
    BTree* tree = (BTree*)btree;
    if (tree == NULL)
        return;
    
    destroy_nodes(tree->head, destroy);
    free(tree);
}

void btree_clear(void* btree, void(*destroy)(void*)) {
    BTree* tree = (BTree*)btree;
    if (tree == NULL)
        return;

    destroy_nodes(tree->head, destroy);
    tree->elemCount = 0;
    tree->head = NULL;
}

size_t btree_count(const void* btree) {
    BTree* tree = (BTree*)btree;
    if (tree == NULL)
        return INVALID;
    return tree->elemCount;
}

static void* create_node(const void* key, size_t valSize, size_t keySize) {
    if (key == NULL || valSize == 0 || keySize == 0)
        return NULL;

    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL)
        return NULL;
    
    newNode->item.key = malloc(keySize);
    if (newNode->item.key == NULL) {
        free(newNode);
        return NULL;
    }

    newNode->item.value = malloc(valSize);
    if (newNode->item.value == NULL) {
        free((void*)newNode->item.key);
        free(newNode);
        return NULL;
    }

    memcpy((void*)newNode->item.key, key, keySize);

    newNode->parent = NULL;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

static void* get_by_key(const void* btree, const void* key) {
    BTree* tree = (BTree*)btree;
    if(tree == NULL || key == NULL)
        return NULL;

    Node* curr = tree->head;

    while (curr != NULL) {
        int cmpRes = tree->cmp(curr->item.key, key);

        if (cmpRes == 0)
            break;
        else if (cmpRes > 0)
            curr = curr->left;
        else
            curr = curr->right;
    }
    return curr;
}

void* btree_item(const void* btree, const void* key) {
    if (btree == NULL || key == NULL)
        return NULL;

    Node* node = get_by_key(btree, key);
    if (node == NULL)
        return NULL;

    return node->item.value;
}

void* btree_insert(void* btree, const void* key, bool* createFlag) {
    BTree* tree = (BTree*)btree;
    if (tree == NULL || key == NULL || createFlag == NULL)
        return NULL;

    *createFlag = false;

    if (tree->head == NULL) {
        Node* newNode = (Node*)create_node(key, tree->valueSize, tree->keySize);
        if (newNode == NULL)
            return NULL;

        *createFlag = true;
        tree->elemCount++;
        tree->head = newNode;

        return newNode->item.value;
    }

    Node* curr = tree->head;
    Node* prev;
    int cmpRes = 0;

    do {
        cmpRes = tree->cmp(curr->item.key, key);
        prev = curr;
        
        if (cmpRes == 0)
            return curr->item.value;
        else if (cmpRes > 0)
            curr = curr->left;
        else
            curr = curr->right;
    } while (curr != NULL);

    Node* newItem = (Node*)create_node(key, tree->valueSize, tree->keySize);
    if (newItem == NULL)
        return NULL;

    *createFlag = true;
    tree->elemCount++;

    if (cmpRes > 0)
        prev->left = newItem;
    else
        prev->right = newItem;
    newItem->parent = prev;
    return newItem->item.value;
}

static bool is_left(Node* toCheck) {    
    if (toCheck == NULL)
        return false;

    Node* tmp = toCheck->parent;
    if (tmp == NULL)
        return false;

    return tmp->left == toCheck;
}

static bool is_head(Node* toCheck) {
    if (toCheck == NULL)
        return false;

    if (toCheck->parent == NULL)
        return true;
    return false;
}

void btree_remove(void* btree, const void* key, void(*destroy)(void*)) {
    BTree* tree = (BTree*)btree;
    if (tree == NULL || key == NULL)
        return;

    Node* curr = get_by_key(tree, key);

    btree_erase(tree, (size_t)curr, destroy);
}

size_t btree_first(const void* btree) {
    BTree* tree = (BTree*)btree;
    if (tree == NULL)
        return (size_t)STOP;
    
    Node* curr = tree->head;
    if (curr == NULL)
        return (size_t)STOP;

    while (curr->left != NULL)
        curr = curr->left;
    return (size_t)curr;
}

size_t btree_last(const void* btree) {
    BTree* tree = (BTree*)btree;
    if (tree == NULL)
        return (size_t)STOP;

    Node* curr = tree->head;
    if (curr == NULL)
        return (size_t)STOP;

    while (curr->right != NULL)
        curr = curr->right;
    return (size_t)curr;
}

size_t btree_next(const void* btree, size_t item_id) {
    BTree* tree = (BTree*)btree;
    Node* curr = (Node*)item_id;
    if (tree == NULL || curr == STOP || tree->head == NULL)
        return (size_t)STOP;

    Node* node = (Node*)curr;
    if (node->right == NULL) {
        Node* tmp = node;
        while (tmp != NULL && !is_left(tmp))
            tmp = tmp->parent;

        if (tmp == NULL)
            return (size_t)STOP;
        else
            return (size_t)tmp->parent;
    }
    else {
        Node* tmp = node->right;
        while (tmp->left != NULL)
            tmp = tmp->left;
        return (size_t)tmp;
    }
}

size_t btree_prev(const void* btree, size_t item_id) {
    BTree* tree = (BTree*)btree;
    Node* curr = (Node*)item_id;
    if (tree == NULL || curr == STOP || tree->head == NULL)
        return (size_t)STOP;

    Node* node = (Node*)curr;
    if (node->left == NULL) {
        Node* tmp = node;
        while (tmp != NULL && is_left(tmp))
            tmp = tmp->parent;

        if (tmp == NULL)
            return (size_t)STOP;
        else
            return (size_t)tmp->parent;
    }
    else {
        Node* tmp = node->left;
        while (tmp->right != NULL)
            tmp = tmp->right;
        return (size_t)tmp;
    }
}

size_t btree_stop(const void* btree) {
    if (btree == NULL)
        return (size_t)NULL;
    return (size_t)STOP;
}

void* btree_current(const void* btree, size_t item_id) {
    BTree* tree = (BTree*)btree;
    Node* curr = (Node*)item_id;
    if (tree == NULL || curr == STOP)
        return NULL;

    return &((Node*)item_id)->item;
}

void btree_erase(void* btree, size_t item_id, void(*destroy)(void*)) {
    BTree* tree = (BTree*)btree;
    Node* elem = (Node*)item_id;
    if (tree == NULL || elem == STOP)
        return;

    if (destroy != NULL)
        destroy(&(elem->item));
    tree->elemCount--;

    if (elem->left == NULL && elem->right == NULL) {
        if (is_head(elem))
            tree->head = NULL;
        else if (is_left(elem)) 
            elem->parent->left = NULL;
        else 
            elem->parent->right = NULL;

        free((void*)elem->item.key);
        free(elem->item.value);
        free(elem);
    }
    else if (elem->left == NULL) {
        
        if (is_head(elem)) {
            tree->head = elem->right;
            elem->right->parent = NULL;
        }
        else if (is_left(elem)) {
            elem->parent->left = elem->right;
            elem->right->parent = elem->parent;
        }
        else {
            elem->parent->right = elem->right;
            elem->right->parent = elem->parent;
        }

        free((void*)elem->item.key);
        free(elem->item.value);
        free(elem);
    }
    else if (elem->right == NULL) {
        
        if (is_head(elem)) {
            tree->head = elem->left;
            elem->left->parent = NULL;
        }
        else if (is_left(elem)) {
            elem->parent->left = elem->left;
            elem->left->parent = elem->parent;
        }
        else {
            elem->parent->right = elem->left;
            elem->left->parent = elem->parent;
        }

        free((void*)elem->item.key);
        free(elem->item.value);
        free(elem);
    }
    else {
        
        Node* tmp = elem->right;
        while (tmp->left != NULL)
            tmp = tmp->left;

        free((void*)elem->item.key);
        free(elem->item.value);
        elem->item.key = tmp->item.key;
        elem->item.value = tmp->item.value;

        if (is_left(tmp) && tmp->right == NULL)
            tmp->parent->left = NULL;
        else if (is_left(tmp) && tmp->right != NULL) {
            tmp->parent->left = tmp->right;
            tmp->right->parent = tmp->parent;
        }
        else if(tmp->right == NULL)
            elem->right = NULL;
        else {
            elem->right = tmp->right;
            tmp->right->parent = elem;
        }

        free(tmp);
    }
}