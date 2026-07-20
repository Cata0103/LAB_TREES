/*Eliminates repeated NULL checks
Makes allocation failures explicit
Keeps focus on structure logic, not error handling
*/
#ifndef XMALLOC_H
#define XMALLOC_H


#include <stdio.h>
#include <stdlib.h>

static inline void *xmalloc(size_t n);
static inline void *xcalloc(size_t n, size_t size);
static inline void *xrealloc(void *p, size_t n);

static inline void *xmalloc(size_t n) {
    void *p = malloc(n);
    if (!p) {
        fprintf(stderr, "Out of memory (malloc %zu bytes)\n", n);
        exit(EXIT_FAILURE);
    }
    return p;
}

static inline void *xcalloc(size_t n, size_t size) {
    void *p = calloc(n, size);
    if (!p) {
        fprintf(stderr, "Out of memory (calloc %zu x %zu)\n", n, size);
        exit(EXIT_FAILURE);
    }
    return p;
}

static inline void *xrealloc(void *p, size_t n) {
    void *q = realloc(p, n);
    if (!q) {
        fprintf(stderr, "Out of memory (realloc %zu bytes)\n", n);
        exit(EXIT_FAILURE);
    }
    return q;
}
#endif