#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "bad.h"

#define VA_INITIAL_CAPACITY 256

/* TODO: Distingish between strong and weak vectors */
/* TODO: Make capacity managment more reasonable */

struct bad_vec_t {
    void **mem;
    size_t final_elem;
    size_t capacity;
    void* (*e_construct)(void*);
    void (*e_destroy)(void*);
    int (*e_compare)(void*, void*);
    bool strong;
};

bool bad_vec_strong_init(
    bad_vec_t **v,
    void* (*e_construct)(void*),
    void (*e_destroy)(void*),
    int (*e_compare)(void*, void*)
)
{
    assert(NULL != v);
    assert(NULL != e_construct);
    assert(NULL != e_destroy);

    *v = calloc(1u, sizeof(bad_vec_t));

    if (NULL == *v)
    {
        return false;
    }

    (*v)->mem = calloc(VA_INITIAL_CAPACITY, sizeof(void*));
    if (NULL == (*v)->mem)
    {
        free(*v);
        return false;
    }

    (*v)->strong = true;
    (*v)->capacity = VA_INITIAL_CAPACITY;
    (*v)->final_elem = 0;
    (*v)->e_construct = e_construct;
    (*v)->e_destroy = e_destroy;
    (*v)->e_compare = e_compare;

    return true;
}

bool bad_vec_weak_init(
    bad_vec_t **v,
    int (*e_compare)(void*, void*)
)
{
    assert(NULL != v);

    *v = calloc(1u, sizeof(bad_vec_t));
    if (NULL == *v)
    {
        return false;
    }

    (*v)->mem = calloc(VA_INITIAL_CAPACITY, sizeof(void*));
    if (NULL == (*v)->mem)
    {
        free(*v);
        return false;
    }

    (*v)->strong = false;
    (*v)->capacity = VA_INITIAL_CAPACITY;
    (*v)->final_elem = 0;
    (*v)->e_compare = e_compare;

    return true;
}

bool bad_vec_destroy(bad_vec_t **v)
{
    if ((*v)->strong)
    {
        bad_vec_map(*v, (*v)->e_destroy);
    }
    free((*v)->mem);
    free(*v);
    *v = NULL;
    return true;
}

bool bad_vec_is_strong(bad_vec_t *v)
{
    assert(NULL != v);
    return v->strong;
}

void bad_vec_push(bad_vec_t *v, void *e)
{
    /*TODO: Make sane, handle weak vs strong */
    assert(NULL != v);
    assert(NULL != e);
    if (v->capacity <= v->final_elem)
    {
        v->mem = realloc(v->mem, sizeof(void*) * (v->capacity * 2));
        assert((NULL != v->mem) && "Here's a penny. Go buy more RAM.");
        v->capacity *= 2;
    }

    if (v->strong)
    {
        v->mem[v->final_elem] = v->e_construct(e);
    }
    else
    {
        v->mem[v->final_elem] = e;
    }

    (v->final_elem)++;
}

void *bad_vec_pop(bad_vec_t *v)
{
    assert(NULL != v);
    assert(v->final_elem > 0);

    (v->final_elem)--;

    return v->mem[v->final_elem];
}

void *bad_vec_elem_at(bad_vec_t *v, size_t i)
{
    assert(NULL != v);
    assert(v->final_elem > i);
    return v->mem + i;
}

void bad_vec_map(bad_vec_t *v, void (*func) (void*))
{
    /* TODO: Null check? */
    assert(NULL != v);
    assert(NULL != func);
    for (size_t i = 0; i < v->final_elem; i++)
    {
        func(*(v->mem + i));
    }
}

void bad_vec_fold(bad_vec_t *v, void *acc, void (*func)(void*, void*))
{
    /* TODO: Null check? */
    assert(NULL != v);
    assert(NULL != func);
    for (size_t i = 0; i < v->final_elem; i++)
    {
        func(acc, *(v->mem + i));
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
