#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

#include "common.h"
#include "rbtree_int.h"
#include "treeint_xt.h"
#include "avltree.h"

struct treeint_ops
{
    void *(*init)();
    int (*destroy)(void *);
    int (*insert)(void *, int);
    void *(*find)(void *, int);
    int (*remove)(void *, int);
    int (*height)(void *);
};

static struct treeint_ops *ops;

static struct treeint_ops xt_ops = {
    .init = treeint_xt_init,
    .destroy = treeint_xt_destroy,
    .insert = treeint_xt_insert,
    .find = treeint_xt_find,
    .remove = treeint_xt_remove,
    .height = treeint_xt_hint,
};

static struct treeint_ops rb_ops = {
    .init = rbtree_init,
    .insert = rbtree_insert,
    .find = rbtree_find,
    .remove = rbtree_remove,
    .height = rbtree_Height,
};

#define rand_key(sz) rand() % ((sz) - 1)

#define bench(statement)                                                 \
    ({                                                                   \
        struct timespec _tt1, _tt2;                                      \
        clock_gettime(CLOCK_MONOTONIC, &_tt1);                           \
        statement;                                                       \
        clock_gettime(CLOCK_MONOTONIC, &_tt2);                           \
        long long time = (long long)(_tt2.tv_sec * 1e9 + _tt2.tv_nsec) - \
                         (long long)(_tt1.tv_sec * 1e9 + _tt1.tv_nsec);  \
        time;                                                            \
    })

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("usage: treeint <tree size> <seed>\n");
        return -1;
    }

    /* Add an option to specify tree implementation */
    ops = &rb_ops;

    size_t tree_size = 0;
    if (!sscanf(argv[1], "%ld", &tree_size))
    {
        printf("Invalid tree size %s\n", argv[1]);
        return -3;
    }

    /* Note: seed 0 is reserved as special value, it will
     * perform linear operatoion. */
    size_t seed = 0;
    if (!sscanf(argv[2], "%ld", &seed))
    {
        printf("Invalid seed %s\n", argv[2]);
        return -3;
    }

    srand(seed);

    void *ctx = ops->init();

    long long insert_time = 0;
    for (size_t i = 0; i < tree_size; ++i)
    {
        int v = seed ? rand_key(tree_size) : i;
        insert_time += bench(ops->insert(ctx, v));
    }
    printf("Red-Black Tree\nAverage insertion time : %lf\n",
           (double)insert_time / tree_size);

    // int rbtree_height = ops->height(ctx);
    // printf("Tree height = %d", rbtree_height);

    long long find_time = 0;
    // printf("v = ");
    for (size_t i = 0; i < tree_size; ++i)
    {
        int v = seed ? rand_key(tree_size) : i;
        // printf("%d ", v);
        find_time += bench(ops->find(ctx, v));
    }
    printf("\nAverage find time : %lf\n", (double)find_time / tree_size);

    /* Removing */
    long long remove_time = 0;
    for (size_t i = 0; i < tree_size; ++i)
    {
        int v = seed ? rand_key(tree_size) : i;
        remove_time += bench(ops->remove(ctx, v));
    }
    printf("Average remove time : %lf\n", (double)remove_time / tree_size);
    printf("\n");

    /* Red-black tree */
    ops = &xt_ops;

    ctx = ops->init(); // pointer to xtree

    insert_time = 0;
    for (size_t i = 0; i < tree_size; ++i)
    {
        int v = seed ? rand_key(tree_size) : i;
        insert_time += bench(ops->insert(ctx, v));
    }

    printf("XTree\nAverage insertion time : %lf\n",
           (double)insert_time / tree_size);

    // print xtree root hint
    ops->height(ctx);

    find_time = 0;
    for (size_t i = 0; i < tree_size; ++i)
    {
        int v = seed ? rand_key(tree_size) : i;
        find_time += bench(ops->find(ctx, v));
    }
    printf("Average find time : %lf\n", (double)find_time / tree_size);

    remove_time = 0;
    for (size_t i = 0; i < tree_size; ++i)
    {
        int v = seed ? rand_key(tree_size) : i;
        remove_time += bench(ops->remove(ctx, v));
    }
    printf("Average remove time : %lf\n", (double)remove_time / tree_size);

    ops->destroy(ctx);

    /* AVL tree */
    struct avlNode *root = NULL;

    insert_time = 0;
    for (size_t i = 0; i < tree_size; ++i)
    {
        int v = seed ? rand_key(tree_size) : i;
        insert_time += bench({ root = insert(root, v); });
    }

    printf("\nAVLTree\nAverage insertion time : %lf\n",
           (double)insert_time / tree_size);

    printf("AVL Tree height : %d\n", get_avl_Height(root));

    find_time = 0;
    for (size_t i = 0; i < tree_size; ++i)
    {
        int v = seed ? rand_key(tree_size) : i;
        find_time += bench(findNode(root, v));
    }
    printf("Average find time : %lf\n", (double)find_time / tree_size);

    remove_time = 0;
    for (size_t i = 0; i < tree_size; ++i)
    {
        int v = seed ? rand_key(tree_size) : i;
        remove_time += bench({ root = deleteNode(root, v); });
    }
    printf("Average remove time : %lf\n", (double)remove_time / tree_size);

    return 0;
}
