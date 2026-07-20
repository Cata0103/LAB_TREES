#ifndef ARRAY
#define ARRAY

#include <stdint.h>
#include <stddef.h>

typedef struct
{
    long * base;
    long size;
    size_t memory_bytes;
} array;

array * create_array(long size);
void clear_array(array * a);
void insert_items(array * a, long limit);
void insert_item_not_sorted(array * a, long item, long i);
long get_item(array * a, long i);

#endif