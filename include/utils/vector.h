#ifndef VECTOR_H

#define VECTOR_H
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct _vector {
    size_t base;
    unsigned long long size;
    unsigned long long max_size;
    void *nodes;
} Vector;

Vector _vector_new(size_t base);

void _vector_push_back(Vector *v, void *val);

void *_vector_get(Vector *v, unsigned long long n);

void _vector_clear(Vector *v, void (*item_rm)(void *item));

void _vector_destory(Vector *v, void (*item_rm)(void *item));
void _vector_sort(Vector *v, int (*cmp)(const void *a, const void *b));

#define vector_new(TYPE) (_vector_new(sizeof(TYPE)))
#define vector_push_back(v, val)                \
    {                                           \
        typeof(val) __tmp_for_copy = val;       \
        _vector_push_back(&v, &__tmp_for_copy); \
    }
#define vector_get(v, n, TYPE) (*(TYPE *)_vector_get(&v, n))
#define vector_clear(v, item_rm) (_vector_clear(&v, item_rm))
#define vector_destory(v, item_rm) (_vector_destory(&v, item_rm))
#define vector_sort(v, cmp) (_vector_sort(&v, cmp))

#endif