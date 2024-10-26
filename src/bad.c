#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "bad.h"

#define VA_INITIAL_CAPACITY 256

/* TODO: Use calloc */
/* TODO: Make capacity managment more reasonable */

struct bad_vec_t {
    char *mem;
    size_t elem_size;
    size_t elems;
    size_t capacity;
    void (*elem_destructor)(void*);
};

bool bad_vec_init(
    bad_vec_t **v,
    size_t elem_size,
    void (*elem_destructor)(void*)
)
{
    assert(NULL != v);
    *v = malloc(sizeof(bad_vec_t));
    if (NULL == *v)
    {
        return false;
    }

    (*v)->elem_size = elem_size;
    (*v)->mem = malloc(elem_size * VA_INITIAL_CAPACITY);
    if (NULL == (*v)->mem)
    {
        free(*v);
        return false;
    }

    (*v)->capacity = VA_INITIAL_CAPACITY;
    (*v)->elems = 0;

    (*v)->elem_destructor = elem_destructor;
    return true;
}

bool bad_vec_destroy(bad_vec_t **v)
{
    assert(NULL != *v);
    if (NULL != (*v)->elem_destructor)
    {
        bad_vec_foreach((*v), (*v)->elem_destructor);
    }
    free(*v);
    *v = NULL;
    return true;
}

void bad_vec_push(bad_vec_t *v, void *e)
{
    /*TODO: Make sane */
    assert(NULL != v);
    assert(NULL != e);
    if (v->capacity <= v->elems)
    {
        v->mem = realloc(v->mem, v->elem_size * (v->capacity * 2));
        assert((NULL != v->mem) && "Here's a penny. Go buy more RAM.");
        v->capacity *= 2;
    }
    memcpy(v->mem + (v->elems * v->elem_size), e, v->elem_size);
    v->elems += 1;
}

char *bad_vec_elem_at(bad_vec_t *v, size_t i)
{
    assert(NULL != v);
    assert(v->elems > i);
    return v->mem + (v->elem_size * i);
}

void bad_vec_foreach(bad_vec_t *v, void (*func) (void*))
{
    assert(NULL != v);
    assert(NULL != func);
    for (size_t i = 0; i < v->elems; i++)
    {
        func(v->mem + (i * v->elem_size));
    }
}

char *bad_strcat(const char *dest, const char *src)
{
    size_t newlen = strlen(dest) + strlen(src) + 1u;

    char *retval = malloc(newlen * sizeof(char));
    assert(retval != NULL);

    strncpy(retval, dest, strlen(dest));
    strncpy(retval + strlen(dest), src, strlen(src));
    retval[newlen] = '\0';

    return retval;
}

uint16_t num_digits(uintmax_t n)
{
    uint16_t retval = 1u;
    while (n > 10u)
    {
        n %= 10u;
        retval++;
    }
    return retval;
}
