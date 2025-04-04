#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
/* Include source file directly to test opaque structures and static methods */
#include "bad.c"

void baseline_tests(void);
void filter_tests(void);
void sort_tests(void);

/* Helper functions */
void string_to_upper(void *e);
void sum_string_lengths(void *acc, void *e);
void *construct_string(void *str);
int compare_string(const void *a, const void *b);
bool is_palindrome(void *e);

#define NUM_TEST_STRINGS 9u
static char *test_strings[] = {
    "sator arepo tenet opera rotas",
    "What indeed could be more monstous than to assert that things could "
    "become better by losing all their goodness?",
    "From festering sockets in his spine fine copper wires trail in a fan",
    "There could be no things but nameless things, no names but thingless "
    "names",
    "able was i ere i saw elba",
    "Not that which must be reached in order to alter him, but that which "
    "must be left intact in order to respect him.",
    "Let that fear what is capable of being affected!",
    "I was flesh, and a breath that passeth away and cometh not again.",
    "Come that day neither to destory nor to protect, but to bear witness."
};

int main(void)
{
    baseline_tests();
    filter_tests();
    sort_tests();

    return 0;
}

void baseline_tests(void)
{
    /* Just create and destroy a weak vector, do nothing else */
    bad_vec_t *x;
    bad_vec_weak_init(&x, NULL);

    assert(VA_INITIAL_CAPACITY == x->capacity);
    assert(!bad_vec_is_strong(x));
    assert(0u == x->final_elem);
    assert(NULL == x->e_construct);
    assert(NULL == x->e_destroy);
    assert(NULL == x->e_compare);

    bad_vec_destroy(&x);
    assert(NULL == x);

    /* Push a few elements to a strong vector and "map" them */
    bad_vec_t *y;
    bad_vec_strong_init(&y, construct_string, free, compare_string);

    assert(bad_vec_is_strong(y));
    for(size_t i = 0u; i < NUM_TEST_STRINGS; i++)
    {
        bad_vec_push(y, test_strings[i]);
    }
    assert(NUM_TEST_STRINGS == y->final_elem);

    for(size_t i = 0u; i < NUM_TEST_STRINGS; i++)
    {
        char *elem = bad_vec_elem_at(y, i);
        assert(0 == strcmp(elem, test_strings[i]));
    }

    bad_vec_map(y, string_to_upper);
    assert(0 == strcmp(
        (char*) bad_vec_elem_at(y, 4),
        "ABLE WAS I ERE I SAW ELBA"
    ));

    size_t strlen_accumulator = 0;
    bad_vec_fold(y, &strlen_accumulator, sum_string_lengths);
    assert(602u == strlen_accumulator);

    char *popped = bad_vec_pop(y);
    assert(0u == strcmp(
        popped,
        "COME THAT DAY NEITHER TO DESTORY NOR TO PROTECT, BUT TO BEAR WITNESS."
    ));
    free(popped);

    assert((NUM_TEST_STRINGS - 1u) == y->final_elem);

    /* Folding with a NULL accumulator should be allowed if the callback
     * does not reference the accumulator. In that case the fold is equivalent
     * to a map. */

    bad_vec_destroy(&y);
    assert(NULL == y);
}

void filter_tests(void)
{
    bad_vec_t *v;
    bad_vec_strong_init(&v, construct_string, free, compare_string);

    for(size_t i = 0u; i < NUM_TEST_STRINGS; i++)
    {
        bad_vec_push(v, test_strings[i]);
    }
    assert(NUM_TEST_STRINGS == v->final_elem);

    bad_vec_t *filtered;
    assert(bad_vec_filter(v, is_palindrome, &filtered));
    assert(2u == filtered->final_elem);
    char *s = bad_vec_pop(filtered);
    assert(0 == strcmp("able was i ere i saw elba", s));
    free(s);
    
    s = bad_vec_pop(filtered);
    assert(0 == strcmp("sator arepo tenet opera rotas", s));
    free(s);
    assert(0u == filtered->final_elem);

    bad_vec_destroy(&v);
    bad_vec_destroy(&filtered);
}

void sort_tests(void)
{
    bad_vec_t *v;
    bad_vec_weak_init(&v, compare_string);
    for(size_t i = 0u; i < NUM_TEST_STRINGS; i++)
    {
        bad_vec_push(v, test_strings[i]);
    }
    assert(NUM_TEST_STRINGS == v->final_elem);

    bad_vec_sort(v);
    assert(NUM_TEST_STRINGS == bad_vec_size(v));

    for(size_t i = 1u; i < NUM_TEST_STRINGS; i++)
    {
        assert(
            0 > strcmp(
                (char *) bad_vec_elem_at(v, i - 1u),
                (char *) bad_vec_elem_at(v, i)
            )
        );
    }

    bad_vec_destroy(&v);
}

void string_to_upper(void *e)
{
    char *ptr = (char*) e;
    while (*ptr)
    {
        *ptr = toupper(*ptr);
        ptr++;
    }
}

void sum_string_lengths(void *acc, void *e)
{
    char *ptr = (char*) e;
    size_t *a = (size_t*) acc;

    (*a) += strlen(ptr);
}

void *construct_string(void *str)
{
    return (void*) strdup((char*)str);
}

int compare_string(const void *a, const void *b)
{
    const char **x = (const char **) a;
    const char **y = (const char **) b;

    return strcmp(*x, *y);
}

bool is_palindrome(void *e)
{
    const char *s = e;
    size_t len = strlen(s);
    for (size_t i = 0u; i <= len / 2u; i++)
    {
        if (s[i] != s[len - (i + 1u)])
        {
            return false;
        }
    }
    return true;
}

