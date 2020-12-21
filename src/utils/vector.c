#include "vector.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

Vector _vector_new(size_t base) {
    DEBUG("new vector!");
    Vector v;
    v.size = 0;
    v.base = base;
    v.max_size = 1;
    v.nodes = malloc(base);
    return v;
}

void _vector_push_back(Vector *v, void *val) {
    DEBUG("push back [%lld]", v->size);
    if (v->size >= v->max_size) {
        DEBUG("realloc! size: %lld * %ld = %lld", v->max_size * 2, v->base, v->max_size * 2 * v->base);
        void *ptr = realloc(v->nodes, v->max_size * 2 * v->base);
        assert(ptr != NULL);
        // memcpy(ptr, v->nodes, v->size * v->base);
        v->nodes = ptr;
        v->max_size *= 2;
    }
    memcpy(v->nodes + v->size * v->base, val, v->base);
    (v->size) += 1;
    DEBUG("after push: v.size = %lld", v->size);
}

void *_vector_get(Vector *v, unsigned long long n) {
    assert(n < v->size);
    return (v->nodes + n * v->base);
}

void _vector_clear(Vector *v, void (*item_rm)(void *item)) {
    for (unsigned long long i = 0; i < v->size; i++) {
        if (item_rm != NULL) {
            item_rm(_vector_get(v, i));
        }
    }
    v->size = 0;
    bzero(v->nodes, v->size*v->base);
}

void _vector_destory(Vector *v, void (*item_rm)(void *item)) {
    if (item_rm != NULL) {
        for (unsigned long long i = 0; i < v->size; i++) {
            item_rm(_vector_get(v, i));
        }
    }
    if (v->nodes != NULL)
        free(v->nodes);
}

void _vector_sort(Vector *v, int (*cmp)(const void *a, const void *b)) {
    qsort(v->nodes, v->size, v->base, cmp);
}
