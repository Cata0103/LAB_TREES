#ifndef NODE_TREE
#define NODE_TREE

#include "node.h"
#include "array.h"
#include<stdint.h>

#define max(a,b) ((a>b) ? a : b)
#define min(a,b) ((a<b) ? a : b)

typedef struct
{
    node * root;
    int32_t size;
} node_tree;

node_tree * tree_set();
node * node_set();
int32_t contains_rec(int32_t val, node * n);
int32_t search(int32_t val, node_tree * tree);
int32_t get_height(node * n);
void update_height(node * n);
node * left_rotate(node * n);
node * right_rotate(node * n);
int32_t balance_factor(node * n);
node * balance(node * n);
node * insert_rec(int32_t val, node * n);
long insert(int32_t val, node_tree * tree);
node * delete_rec(int32_t val, node * n);
int32_t delete(int32_t val, node_tree * tree);
void clear_node(node * n);
void clear_tree(node_tree * tree);
void print_tree(node_tree * tree);
void print_node(node * n, array * a);
void range_search_rec(node * n,array * range_array, int32_t a, int32_t b);
int32_t range_search(node_tree * tree,int32_t a, int32_t b);
long get_nodes_visited(void);
long get_nodes_printed(void);

#endif