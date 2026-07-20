#include "xmalloc.h"
#include "array.h"
#include "assertions.h"
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

array * create_array(long size){
    array * a = (array *)xmalloc(sizeof(array));
    a -> base = (long *)xmalloc(sizeof(long)*size);
    a -> size = size;
    return a;
}

void clear_array(array * a){
    free(a->base);
    free(a);
}

void insert_items(array * a, long limit){
    srand(time(0));
    long element;
    long first_rand = (long) rand();
    for(long i = 0; i < a->size;i++){
        element = (long) rand() % (limit);
        a->base[i] = element;
    }
}

void insert_item_not_sorted(array * a, long item, long i){
    a->base[i] = item;
}

long get_item(array * a, long i){
    return a->base[i];
}