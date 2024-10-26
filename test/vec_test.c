#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
/* Include source file directly to test opaque structures and static methods */
#include "bad.c"

void double_vec_elems(void *e);

int main(void)
{
    /* Just create and destroy a vec, do nothing else */
    bad_vec_t *x;
    bad_vec_init(&x, sizeof(char), free);

    assert(VA_INITIAL_CAPACITY == x->capacity);
    assert(sizeof(char) == x->elem_size);
    assert(0u == x-> elems);

    bad_vec_destroy(&x);
    assert(NULL == x);

    /* Push a few elements and foreach them */
    bad_vec_t *y;
    bad_vec_init(&y, sizeof(uint32_t), free);

    assert(sizeof(uint32_t) == x->elem_size);
    uint32_t uint32_arr[] = {0,1,2,3,4,5,6,7};
    for(size_t i = 0; i < sizeof(uint32_arr); i++)
    {
        bad_vec_push(y, &uint32_arr[i]);
        assert(*((uint32_t*) bad_vec_elem_at(y, i)) == uint32_arr[i]);
    }
    assert(8u == x-> elems);

    bad_vec_foreach(y, double_vec_elems);
    for(size_t i = 0; i < sizeof(uint32_arr); i++)
    {
        assert(*((uint32_t*) bad_vec_elem_at(y, i)) == (uint32_arr[i] * 2));
    }

    bad_vec_destroy(&y);
    assert(NULL == y);

    return 0;
}

void double_vec_elems(void *e)
{
    uint32_t *ptr = (uint32_t*) e;
    *ptr *= 2;
}
