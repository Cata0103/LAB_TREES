#include "node_tree.h"
#include "node.h"
#include "xmalloc.h"
#include <stdint.h>
#include <stdlib.h>
#include "array.h"

int k = 0;
long rotation_count = 0;
long nodes_visited = 0;
long nodes_printed = 0;

node_tree * tree_set(){
    node_tree * tree = (node_tree*)xmalloc(sizeof(node_tree));
    tree ->root = NULL;
    tree->size = 0;
    return tree;
}

node * node_set(){
    node * n = (node*)xmalloc(sizeof(node));
    n->height=0;
    n->value=0;
    n->parent=NULL;
    n->left=NULL;
    n->right=NULL;
    return n;
}

int32_t contains_rec(int32_t val, node * n){
    if(n == NULL){
        return 0;
    }
    if(n->value == val){
        return 1;
    }
    if(n->value > val){
        nodes_visited++;
        return contains_rec(val, n->left);
    }
    else{
        nodes_visited++;
        return contains_rec(val, n->right);
    }
}

int32_t search(int32_t val, node_tree * tree){
    nodes_visited = 0;
    return contains_rec(val, tree->root);
}

int32_t get_height(node * n){
    if(n == NULL){
        return -1;
    }
    return n->height;
}

void update_height(node * n){
    n->height = max(get_height(n->left),get_height(n->right)) + 1;
}

node * left_rotate(node * n){
    if(n->right == NULL){
        return n;
    }
    node * pivot = n->right;
    n->right = pivot -> left;
    pivot->left = n;
    update_height(n);
    update_height(pivot);
    return pivot;
}

node * right_rotate(node * n){
    if(n->left == NULL){
        return n;
    }
    node * pivot = n->left;
    n->left = pivot->right;
    pivot->right = n;
    update_height(n);
    update_height(pivot);
    return pivot;
}

int32_t balance_factor(node * n){
    if(n == NULL){
        return 0;
    }
    return get_height(n->left) - get_height(n->right);
}

node * balance(node * n){
    if(abs(balance_factor(n)) <= 1){
        return n;
    }
    if(get_height(n->left) > get_height(n->right)){ // IZQ
        if(get_height(n->left->left) > get_height(n->left->right)){
            rotation_count++;
            return right_rotate(n);
        } else {
            n->left = left_rotate(n->left);
            rotation_count++;
            rotation_count++;
            return right_rotate(n);
        }
    }else{
        if(get_height(n->right->left) > get_height(n->right->right)) {
            n->right = right_rotate(n->right);
            rotation_count++;
            rotation_count++;
            return left_rotate(n);
        } else {
            rotation_count++;
            return left_rotate(n);
        }
    }
}

node * insert_rec(int32_t val, node * n){
    if(n==NULL){
        node * new_node = node_set();
        new_node->value = val;
        return new_node;
    }
    if(val < n->value){
        n->left = insert_rec(val, n->left);
        n->left->parent = n->left;
    }
    else{
        n->right = insert_rec(val, n->right);
        n->right->parent = n->right;
    }
    n = balance(n);
    update_height(n);
    return n;
}

long insert(int32_t val, node_tree * tree){
    rotation_count = 0;
    if(search(val, tree)){
        return -1;
    }
    tree->root = insert_rec(val, tree->root);
    tree->size++;
    return rotation_count;
}

node * delete_rec(int32_t val, node * n){
    if(val < n->value){
        n->left = delete_rec(val, n->left);
    } else if(val > n->value){
        n->right = delete_rec(val, n->right);
    } else {
        if(n->left == NULL && n->right == NULL){
            return NULL;
        }
        if(n->right == NULL){
            return n->left;
        }
        if(n->left == NULL){
            return n->right;
        }
        node * suc = n->right;
        while(suc->left != NULL){
            suc = suc->left;
        }
        n->value = suc->value;
        n->right = delete_rec(suc->value, n->right);
    }
    n = balance(n);
    update_height(n);
    return n;
}

int32_t delete(int32_t val, node_tree * tree){
    if(search(val, tree) == 0){
        return 0;
    }
    tree -> root = delete_rec(val,tree->root);
    tree->size--;
    return 1;
}

void clear_node(node * n){
    if(n == NULL){
        return;
    }
    clear_node(n->left);
    clear_node(n->right);
    free(n);
}

void clear_tree(node_tree * tree){
    node * parent = tree->root;
    clear_node(parent);
    free(tree);
}

void print_node(node * n, array * a){
    if(n == NULL){
        return;
    }
    print_node(n->left, a);
    insert_item_not_sorted(a, n->value, k++);
    print_node(n->right, a);
}

void print_tree(node_tree * tree){
    node * parent = tree->root;
    array * traversal_array = create_array(tree->size);
    // printf("{ ");
    print_node(parent, traversal_array);
    // for(long i = 0; i < traversal_array->size; i++){
    //     printf("%ld ", get_item(traversal_array, i));
    // }
    // printf("}\n");
    clear_array(traversal_array);
    k = 0;
}

void range_search_rec(node * n,array * range_array, int32_t a, int32_t b){
    if(n == NULL){
        return;
    }
    if(n->value > a){
        nodes_visited++;
        range_search_rec(n->left, range_array, a,b);
    }if(a <= n->value && n->value <= b){
        insert_item_not_sorted(range_array, n->value, k++);
        nodes_printed++;
    }
    if(n->value < b ){
        nodes_visited++;
        range_search_rec(n->right, range_array, a,b);
    }
}

int32_t range_search(node_tree * tree, int32_t a, int32_t b){
    nodes_visited = 0;
    nodes_printed = 0;
    array * range_array = create_array(tree->size);
    if(tree == NULL){
        return 0;
    }
    // printf("{ ");
    range_search_rec(tree->root,range_array,a,b);
    // for(long i = 0; i < k; i++){
    //     printf("%ld ", get_item(range_array, i));
    // }
    // printf("}\n");
    clear_array(range_array);
    k = 0;
    return 1;
}

long get_nodes_visited(void){
    return nodes_visited;
}

long get_nodes_printed(void){
    return nodes_printed;
}
