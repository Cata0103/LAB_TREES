#ifndef LEAF_TREE
#define LEAF_TREE

typedef int object_t;
typedef int key_t;
typedef struct range_return_t{
    long visited_nodes;
    long printed_nodes;
} range_return_t;

typedef struct tr_n_t { 
    key_t        key;
    struct tr_n_t  *left;
    struct tr_n_t *right;
    struct tr_n_t *left_link;
    struct tr_n_t *right_link;
    int           height; 
} tree_node_t;

tree_node_t *get_node_leaf();
void return_node_leaf(tree_node_t *node);
tree_node_t *create_tree_leaf(void);
void return_tree_leaf(tree_node_t * leaf_tree);
void left_rotation_leaf(tree_node_t *n);
void right_rotation_leaf(tree_node_t *n);
long find_leaf(tree_node_t *tree, key_t query_key);
int insert_leaf(tree_node_t *tree, key_t new_key, object_t *new_object);
object_t *delete_node_leaf(tree_node_t *tree, key_t delete_key);
void print_list_ordered_leaf(tree_node_t * n);
void print_list_inverted_leaf(tree_node_t * n);
void print_node_leaf(tree_node_t * n);
void print_tree_leaf(tree_node_t * tree);
range_return_t range_search_leaf(tree_node_t * tree, int a, int b);

#endif