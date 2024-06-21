#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "xtree.h"

struct avlNode
{
    int key;
    struct avlNode *left;
    struct avlNode *right;
    int height;
};

int get_avl_Height(struct avlNode *n)
{
    if (n == NULL)
        return 0;
    return n->height;
}

struct avlNode *createNode(int key)
{
    struct avlNode *node = (struct avlNode *)malloc(sizeof(struct avlNode));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

int getBalanceFactor(struct avlNode *n)
{
    if (n == NULL)
    {
        return 0;
    }
    return get_avl_Height(n->left) - get_avl_Height(n->right);
}

struct avlNode *rightRotate(struct avlNode *y)
{
    struct avlNode *x = y->left;
    struct avlNode *T2 = x->right;

    x->right = y;
    y->left = T2;

    x->height = max(get_avl_Height(x->right), get_avl_Height(x->left)) + 1;
    y->height = max(get_avl_Height(y->right), get_avl_Height(y->left)) + 1;

    return x;
}

struct avlNode *leftRotate(struct avlNode *x)
{
    struct avlNode *y = x->right;
    struct avlNode *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(get_avl_Height(x->right), get_avl_Height(x->left)) + 1;
    y->height = max(get_avl_Height(y->right), get_avl_Height(y->left)) + 1;

    return y;
}

struct avlNode *insert(struct avlNode *node, int key)
{
    if (node == NULL)
        return createNode(key);

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else
        return node;

    node->height = 1 + max(get_avl_Height(node->left), get_avl_Height(node->right));
    int bf = getBalanceFactor(node);

    // Left Left Case
    if (bf > 1 && key < node->left->key)
    {
        return rightRotate(node);
    }
    // Right Right Case
    if (bf < -1 && key > node->right->key)
    {
        return leftRotate(node);
    }
    // Left Right Case
    if (bf > 1 && key > node->left->key)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    // Right Left Case
    if (bf < -1 && key < node->right->key)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

struct avlNode *findNode(struct avlNode *root, int value)
{
    if (root == NULL || root->key == value)
        return root;

    if (value < root->key)
        return findNode(root->left, value);

    return findNode(root->right, value);
}

struct avlNode *minValueNode(struct avlNode *node)
{
    struct avlNode *current = node;

    /* loop down to find the leftmost leaf */
    while (current->left != NULL)
        current = current->left;

    return current;
}

// Recursive function to delete a node
// with given key from subtree with
// given root. It returns root of the
// modified subtree.
struct avlNode *deleteNode(struct avlNode *root, int key)
{

    // STEP 1: PERFORM STANDARD BST DELETE
    if (root == NULL)
        return root;

    // If the key to be deleted is smaller
    // than the root's key, then it lies
    // in left subtree
    if (key < root->key)
        root->left = deleteNode(root->left, key);

    // If the key to be deleted is greater
    // than the root's key, then it lies
    // in right subtree
    else if (key > root->key)
        root->right = deleteNode(root->right, key);

    // if key is same as root's key, then
    // This is the node to be deleted
    else
    {
        // node with only one child or no child
        if ((root->left == NULL) ||
            (root->right == NULL))
        {
            struct avlNode *temp = root->left ? root->left : root->right;

            // No child case
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else               // One child case
                *root = *temp; // Copy the contents of
                               // the non-empty child
            free(temp);
        }
        else
        {
            // node with two children: Get the inorder
            // successor (smallest in the right subtree)
            struct avlNode *temp = minValueNode(root->right);

            // Copy the inorder successor's
            // data to this node
            root->key = temp->key;

            // Delete the inorder successor
            root->right = deleteNode(root->right,
                                     temp->key);
        }
    }
}

void preOrder(struct avlNode *root)
{
    if (root != NULL)
    {
        printf("%d ", root->key);
        preOrder(root->left);
        preOrder(root->right);
    }
}

// 计算 AVL 树的最大高度
int calculateMaxHeight(int n)
{
    return ceil(1.44 * log2(n + 2) - 0.328);
}

// 计算 AVL 树的最小高度
int calculateMinHeight(int n)
{
    return floor(log2(n + 1));
}

// 计算 AVL 树的高度区间
void calculateAVLHeightRange(int n, int *min_height, int *max_height)
{
    *min_height = calculateMinHeight(n);
    *max_height = calculateMaxHeight(n);
}

// int main()
// {
//     // struct avlNode *root = NULL;
//     // for (int j = 1; j <= 100; j++)
//     // {
//     //     root = insert(root, j);
//     // }

//     // int i = get_avl_Height(root);
//     // printf("Height : %d\n", i);
//     // preOrder(root);

//     struct avlNode *root = NULL;
//     int keys[] = {64, 20, 88, 83, 93, 79, 29, 54, 34, 45, 65, 29, 67, 26, 18, 33, 68, 87, 48, 39, 40, 39, 69, 31, 65, 31, 69, 35, 60, 26, 0, 26, 46, 86, 10, 38, 64, 39, 90, 0, 82, 57, 29, 50, 81, 45, 81, 51, 32, 28, 88, 72, 68, 59, 4, 32, 90, 73, 66, 50, 97, 64, 74, 44, 51, 84, 82, 17, 22, 72, 15, 5, 30, 42, 54, 10, 88, 36, 59, 21, 65, 49, 91, 32, 9, 93, 62, 97, 67, 29};
//     int num_keys = sizeof(keys) / sizeof(keys[0]);

//     for (int i = 0; i < num_keys; ++i)
//     {
//         root = insert(root, keys[i]);
//     }
//     int i = get_avl_Height(root);
//     printf("Height : %d\n", i);

//     // int n = 100;
//     // int min_height, max_height;
//     // calculateAVLHeightRange(n, &min_height, &max_height);

//     return 0;
// }