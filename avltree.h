#pragma once

#include <stdio.h>
#include <stdlib.h>

struct avlNode
{
    int key;
    struct avlNode *left;
    struct avlNode *right;
    int height;
};

int get_avl_Height(struct avlNode *n);
struct avlNode *createNode(int key);
int max(int a, int b);
int getBalanceFactor(struct avlNode *n);
struct avlNode *rightRotate(struct avlNode *y);
struct avlNode *leftRotate(struct avlNode *x);
struct avlNode *insert(struct avlNode *node, int key);
struct avlNode *findNode(struct avlNode *node, int key);
struct avlNode *deleteNode(struct avlNode *node, int key);
void preOrder(struct avlNode *root);