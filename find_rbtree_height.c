#include <stdio.h>
#include <stdlib.h>

struct rb_node
{
    int key;
    int color;
    struct rb_node *left, *right, *parent;
};

struct rb_root
{
    struct rb_node *node;
};

struct rb_node *create_rb_node(int key)
{
    struct rb_node *node = (struct rb_node *)malloc(sizeof(struct rb_node));
    if (node)
    {
        node->key = key;
        node->color = 0;
        node->left = node->right = node->parent = NULL;
    }
    return node;
}

void left_rotate(struct rb_root *root, struct rb_node *x)
{
    struct rb_node *y = x->right;
    x->right = y->left;
    if (y->left != NULL)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == NULL)
        root->node = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void right_rotate(struct rb_root *root, struct rb_node *y)
{
    struct rb_node *x = y->left;
    y->left = x->right;
    if (x->right != NULL)
        x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == NULL)
        root->node = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;
    x->right = y;
    y->parent = x;
}

void rb_insert_fixup(struct rb_root *root, struct rb_node *z)
{
    while (z != root->node && z->parent->color == 0)
    {
        if (z->parent == z->parent->parent->left)
        {
            struct rb_node *y = z->parent->parent->right;
            if (y && y->color == 0)
            {
                z->parent->color = 1;
                y->color = 1;
                z->parent->parent->color = 0;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->right)
                {
                    z = z->parent;
                    left_rotate(root, z);
                }
                z->parent->color = 1;
                z->parent->parent->color = 0;
                right_rotate(root, z->parent->parent);
            }
        }
        else
        {
            struct rb_node *y = z->parent->parent->left;
            if (y && y->color == 0)
            {
                z->parent->color = 1;
                y->color = 1;
                z->parent->parent->color = 0;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->left)
                {
                    z = z->parent;
                    right_rotate(root, z);
                }
                z->parent->color = 1;
                z->parent->parent->color = 0;
                left_rotate(root, z->parent->parent);
            }
        }
    }
    root->node->color = 1;
}

void rb_insert(struct rb_root *root, int key)
{
    struct rb_node *z = create_rb_node(key);
    struct rb_node *y = NULL;
    struct rb_node *x = root->node;

    while (x != NULL)
    {
        y = x;
        if (z->key < x->key)
            x = x->left;
        else if (z->key > x->key)
            x = x->right;
        else
        {
            free(z);
            return;
        }
    }

    z->parent = y;
    if (y == NULL)
        root->node = z;
    else if (z->key < y->key)
        y->left = z;
    else
        y->right = z;

    z->left = z->right = NULL;
    z->color = 0;

    rb_insert_fixup(root, z);
}

int rb_min_depth(struct rb_node *node)
{
    if (node == NULL)
        return 0;

    int left_depth = rb_min_depth(node->left);
    int right_depth = rb_min_depth(node->right);

    return 1 + (left_depth < right_depth ? left_depth : right_depth);
}

int rb_max_depth(struct rb_node *node)
{
    if (node == NULL)
        return 0;

    int left_depth = rb_max_depth(node->left);
    int right_depth = rb_max_depth(node->right);

    return 1 + (left_depth > right_depth ? left_depth : right_depth);
}

int main()
{
    struct rb_root root = {NULL};

    // sequential input
    for (int i = 0; i < 100; ++i)
    {
        rb_insert(&root, i);
    }

    int min_depth = rb_min_depth(root.node);
    int max_depth = rb_max_depth(root.node);

    printf("Minimum depth of the red-black tree for sequential input: %d\n", min_depth);
    printf("Maximum depth of the red-black tree for sequential input: %d\n", max_depth);

    // random input
    struct rb_root tree = {NULL};
    int values[] = {63, 91, 74, 73, 71, 84, 59, 29, 51, 63, 16, 56, 43, 29, 8, 55, 6, 32, 19, 86, 4, 24, 81, 58, 87, 47, 93, 2, 54, 85, 21, 16, 77, 93, 87, 50, 76, 47, 77, 29, 10, 93, 83, 53, 21, 92, 9, 27, 25, 29, 13, 30, 51, 92, 86, 37, 38, 78, 40, 92, 65, 61, 8, 41, 55, 95, 91, 31, 44, 67, 60, 52, 59, 44, 6, 81, 35, 13, 9, 61, 40, 20, 89, 91, 14, 74, 30, 52, 53, 70, 44, 19, 30, 52, 59, 83, 48, 51, 15, 90};
    int size = sizeof(values) / sizeof(values[0]);

    for (int i = 0; i < size; i++)
    {
        rb_insert(&tree, values[i]);
    }

    int max_height = rb_max_depth(tree.node);
    int min_height = rb_min_depth(tree.node);

    printf("Maximum of RB tree for random input: %d\n", max_height);
    printf("Minimum depth of RB tree for random input: %d\n", min_height);
    return 0;
}