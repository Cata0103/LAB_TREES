#ifndef NODE
#define NODE

#include <stdint.h>

typedef struct t_node
{
    struct t_node * parent;
    int32_t value;
    int32_t height;
    struct t_node * left;
    struct t_node * right;
} node;

#endif