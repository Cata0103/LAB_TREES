#include "node_tree.h"
#include "node.h"
#include "leaf_tree.h"
#include "xmalloc.h"
#include "array.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

// gcc main.c node_tree.c leaf_tree.c array.c -o a

struct timespec t1, t2, t3, t4;

long get_random_interval(long right_limit){
    srand(time(0));
    long random_num = rand() % right_limit;
    return random_num;
}

int main(){
    tree_node_t * leaf_tree;
    node_tree * node_tree;
    array * input_array;
    char action,input_size, tree_choice;
    char tree_chosen = '0';
    double time_elapsed_node, time_elapsed_leaf;

    printf("Lab 2: Leaf-Based AVL Trees and Efficient Range Queries\n");
    node_tree = tree_set();
    leaf_tree = create_tree_leaf();

    printf("Select the input size for the experiment:\n(1) 10000\n(2) 50000\n(3) 100000\n(4) 500000\n");
    while(input_size!='0'){
        scanf("%s", &input_size);
        if(input_size == '1')
        {
            input_array = create_array(10000);
            insert_items(input_array, 100*10000);
            break;
        }
        if(input_size == '2')
        {
            input_array = create_array(50000);
            insert_items(input_array, 100*50000);
            break;
        }
        if(input_size== '3')
        {
            input_array = create_array(100000);
            insert_items(input_array, 100*100000);
            break;
        }
        if(input_size== '4')
        {
            input_array = create_array(500000);
            insert_items(input_array, 100*500000);
            break;
        }
    }

    printf("Inserting items into the chosen tree...\n");
    for(long i = 0;i<input_array->size;i++){
        insert(input_array->base[i],node_tree);
        int *insobj = (int *)xmalloc(sizeof(int));
        *insobj = 10*input_array->base[i];
        insert_leaf(leaf_tree,input_array->base[i],insobj);
    }

    printf("Select one of the available options:\n(i) insertions\n(s) point searches\n(r) range queries\n(p) sequential traversal\n(q) quit\n");
    while( (action = getchar())!= 'q' )
    {
     if( action == 'i' )
     {
        long insertions = 10000;
        long rotation_list_node[insertions], rotation_list_leaf[insertions];
        long average_rotations_node = 0;
        long average_rotations_leaf = 0;
        array * insert_array = create_array(insertions);
        insert_items(insert_array, input_array->size);

        clock_gettime(CLOCK_MONOTONIC, &t1);
        printf("Inserting random keys in the node tree.\n");
        for(long i=0;i<insert_array->size;i++){
            rotation_list_node[i] = insert(get_item(insert_array,i),node_tree);
        }
        clock_gettime(CLOCK_MONOTONIC, &t2);

        clock_gettime(CLOCK_MONOTONIC, &t3);
        printf("Inserting random keys in the leaf tree.\n");
        for(long i=0;i<insert_array->size;i++){
            int *insobj = (int *)xmalloc(sizeof(int));
            *insobj = 10*get_item(insert_array,i);
            rotation_list_leaf[i] = insert_leaf(leaf_tree,get_item(insert_array,i),insobj);
        }
        clock_gettime(CLOCK_MONOTONIC, &t4);

        time_elapsed_node = (t2.tv_sec - t1.tv_sec) + (t2.tv_nsec - t1.tv_nsec) / 1e9;
        time_elapsed_leaf = (t4.tv_sec - t3.tv_sec) + (t4.tv_nsec - t3.tv_nsec) / 1e9;
        long count = 0;

        for(int i=0;i<insertions;i++){
            if(rotation_list_node[i]!= -1){
                average_rotations_node += rotation_list_node[i];
            }else{
                insertions--;
                count++;
            }
        }

        for(int i=0;i<insertions;i++){
            if(rotation_list_leaf[i]!= -1){
                average_rotations_leaf += rotation_list_leaf[i];
            }else{
                insertions--;
                count++;
            }
        }

        printf("Inserted %ld keys into the tree.\n", insertions);
        printf("Number of duplicate keys: %ld\n", count);

        printf("Number of rotations performed node tree: %ld\n", average_rotations_node);
        printf("Number of rotations performed leaf tree: %ld\n", average_rotations_leaf);

        printf("Time elapsed Node Tree: %f seconds\n", time_elapsed_node);
        printf("Time elapsed Leaf Tree: %f seconds\n", time_elapsed_leaf);
        clear_array(insert_array);
     }
     if( action == 's')
     {
        printf("Searching for random keys in the tree.\n");
        long search_key, nodes_visited_list_n[10000], nodes_visited_list_l[10000];
        double average_nodes_visited_n = 0, average_nodes_visited_l = 0;
        array * search_array = create_array(input_array->size-1);
        insert_items(search_array, input_array->size-1);

        //Measure the time and the number of nodes visited
        clock_gettime(CLOCK_MONOTONIC, &t1);
        for(long i=0;i<10000;i++){
            long random_key = get_item(search_array, i);
            // printf("Searching for key: %ld\n", random_key);
            search(random_key, node_tree);
            nodes_visited_list_n[i] = get_nodes_visited();
        }
        clock_gettime(CLOCK_MONOTONIC, &t2);

        clock_gettime(CLOCK_MONOTONIC, &t3);
        for(long i=0;i<10000;i++){
            long random_key = get_item(search_array, i);
            // printf("Searching for key: %ld\n", random_key);
            nodes_visited_list_l[i] = find_leaf(leaf_tree, random_key);
        }
        clock_gettime(CLOCK_MONOTONIC, &t4);

        time_elapsed_node = (t2.tv_sec - t1.tv_sec) + (t2.tv_nsec - t1.tv_nsec) / 1e9;
        time_elapsed_leaf = (t4.tv_sec - t3.tv_sec) + (t4.tv_nsec - t3.tv_nsec) / 1e9;

        for(int i=0;i<10000;i++){
            average_nodes_visited_n += nodes_visited_list_n[i];
        }
        average_nodes_visited_n /= 10000;

        for(int i=0;i<10000;i++){
            average_nodes_visited_l += nodes_visited_list_l[i];
        }
        average_nodes_visited_l /= 10000;

        printf("Average nodes visited (Node Tree): %f\n", average_nodes_visited_n);
        printf("Time elapsed (Node Tree): %f seconds\n", time_elapsed_node);
        printf("Average nodes visited (Leaf Tree): %f\n", average_nodes_visited_l);
        printf("Time elapsed (Leaf Tree): %f seconds\n", time_elapsed_leaf);
        clear_array(search_array);
     }
     if( action == 'r')
     {
        printf("Performing range search in the tree.\n");
        char range_choice;
        range_return_t result_list_node[1000], result_list_leaf[1000];
        long average_nodes_visited_n = 0, average_nodes_visited_l = 0;
        long average_printed_nodes_n = 0, average_printed_nodes_l = 0;
        long aproximate; //How did I get the number? testing and checking the average in the output.
        array * range_array = create_array(1000);
        insert_items(range_array, input_array->size);
        printf("Select the aproximate number of keys to be printed in the range search:\n(1) 10\n(2) 100\n(3) 1000\n(4) 5000\n");
        while(range_choice!='0'){
            scanf("%s", &range_choice);
            if(range_choice == '1')
            {
                printf("Performing range search with 10 keys printed.\n");
                aproximate = 1000;
                break;
            }
            if(range_choice == '2')
            {
                printf("Performing range search with 100 keys printed.\n");
                aproximate = 10000;
                break;
            }
            if(range_choice== '3')
            {
                printf("Performing range search with 1000 keys printed.\n");
                aproximate = 100000;
                break;
            }
            if(range_choice== '4')
            {
                printf("Performing range search with 5000 keys printed.\n");
                aproximate = 500000;
                break;
            }
        }

        clock_gettime(CLOCK_MONOTONIC, &t1);
        for(long i=0;i<1000;i++){
            long random_key = get_item(range_array, i);
            long random_key2 = random_key + aproximate;
            range_search(node_tree, random_key, random_key2);
            result_list_node[i].visited_nodes = get_nodes_visited();
            result_list_node[i].printed_nodes = get_nodes_printed();
        }
        clock_gettime(CLOCK_MONOTONIC, &t2);

        clock_gettime(CLOCK_MONOTONIC, &t3);
        for(long i=0;i<1000;i++){
            long random_key = get_item(range_array, i);
            long random_key2 = random_key + aproximate;
            result_list_leaf[i] = range_search_leaf(leaf_tree, random_key, random_key2);
        }
        clock_gettime(CLOCK_MONOTONIC, &t4);

        for(int i=0;i<1000;i++){
            average_nodes_visited_n += result_list_node[i].visited_nodes;
            average_printed_nodes_n += result_list_node[i].printed_nodes;
        }
        average_nodes_visited_n /= 1000;
        average_printed_nodes_n /= 1000;

        for(int i=0;i<1000;i++){
            average_nodes_visited_l += result_list_leaf[i].visited_nodes;
            average_printed_nodes_l += result_list_leaf[i].printed_nodes;
        }
        average_nodes_visited_l /= 1000;
        average_printed_nodes_l /= 1000;

        printf("Average nodes visited (Node Tree): %ld\n", average_nodes_visited_n);
        printf("Average printed nodes (Node Tree): %ld\n", average_printed_nodes_n);
        printf("Average nodes visited (Leaf Tree): %ld\n", average_nodes_visited_l);
        printf("Average printed nodes (Leaf Tree): %ld\n", average_printed_nodes_l);

        time_elapsed_node = (t2.tv_sec - t1.tv_sec) + (t2.tv_nsec - t1.tv_nsec) / 1e9;
        time_elapsed_leaf = (t4.tv_sec - t3.tv_sec) + (t4.tv_nsec - t3.tv_nsec) / 1e9;
        printf("Time elapsed (Node Tree): %f seconds\n", time_elapsed_node);
        printf("Time elapsed (Leaf Tree): %f seconds\n", time_elapsed_leaf);
        clear_array(range_array);
     }
     if( action == 'p')
     {
        printf("Printing the tree...\n");
        clock_gettime(CLOCK_MONOTONIC, &t1);
        print_tree(node_tree);
        clock_gettime(CLOCK_MONOTONIC, &t2);

        clock_gettime(CLOCK_MONOTONIC, &t3);
        print_list_ordered_leaf(leaf_tree);
        clock_gettime(CLOCK_MONOTONIC, &t4);

        time_elapsed_node = (t2.tv_sec - t1.tv_sec) + (t2.tv_nsec - t1.tv_nsec) / 1e9;
        time_elapsed_leaf = (t4.tv_sec - t3.tv_sec) + (t4.tv_nsec - t3.tv_nsec) / 1e9;
        printf("Time elapsed (Node Tree): %f seconds\n", time_elapsed_node);
        printf("Time elapsed (Leaf Tree): %f seconds\n", time_elapsed_leaf);
     }
     if( action == 'q')
     {
        printf("Closing...\n");
     }
    }
    if(tree_chosen=='1'){
        clear_tree(node_tree);
    }else{
        return_tree_leaf(leaf_tree);
    }
    clear_array(input_array);
    return 0;
}