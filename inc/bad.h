#include <stdint.h>
#include <stdbool.h>

/**
 * A stack-like variable length array that allows for map, filter and fold
 * operations.
 *
 * The ownership model of a bad_vec_t depends on whether it was created using
 * bad_vec_weak_init or bad_vec_strong_init. Weak vectors do not take ownership
 * of the elements passed to them. They merely store a pointer to each
 * individual element. Strong vectors create a copy of the elements placed in
 * them and take ownership of the resulting memory.
 */
typedef struct bad_vec_t bad_vec_t;

/**
 * Create a strong bad_vec_t with 0 elements and set v to point to it. A strong
 * bad_vec_t creates a copy of every element added to it using the e_construct
 * argument. When an element is removed or v is destroyed, said element is
 * deallocated using the e_destroy argument.
 *
 * None of v, e_construct and e_destroy may be NULL. e_compare may be NULL, but
 * calling bad_vec_sort on a bad_vec_t for which e_compare is NULL is
 * undefined.
 */
bool bad_vec_strong_init(
    bad_vec_t **v,
    void (*e_construct)(void*),
    void (e_destroy)(void*),
    int (e_compare)(void*, void*)
);

/**
 * Create a weak bad_vec_t with 0 elements and set v to point to it. A weak
 * bad_vec_t merely stores a pointer to every element added to it without
 * allocating any additional memory and without taking ownership of said
 * element. When an element is removed or v is destroyed, no memory is
 * deallocated except the memory directly associated with v.
 *
 * v may not be NULL. e_compare may be NULL, but calling bad_vec_sort on a
 * bad_vec_t for which e_compare is NULL is undefined.
 */
bool bad_vec_weak_init(bad_vec_t **v, int (e_compare)(void*, void*));

/**
 * Deallocate the memory associated with v. If v is strong, every element of v
 * is additionally deallocated using the e_destroy callback provided when v was
 * created. Otherwise, if v is weak, only the memory of v itself is deallocated
 * and no action is taken on any of the elements of v.
 */
bool bad_vec_destroy(bad_vec_t **v);

/**
 * Place a new element into v, immediately after the current final element in
 * v. If v is strong, e is copied using the e_construct callback provided when
 * v was created. Otherwise, if v is weak, only a pointer to e is stored and no
 * additional memory is allocated.
 *
 * If v is at full capacity upon this function being called, an attempt is made
 * to increase the capacity of v.
 */
void bad_vec_push(bad_vec_t *v, void *e);

/**
 * Remove the final element of v, returning a pointer to said element. If v is
 * a strong vector, the calling function now owns the memory associated with the
 * popped element and is responsible for freeing it. If v is a weak vector, the
 * owner of the popped element remains unchanged after this function is called.
 *
 * The result of calling this function on a vector with 0 elements is
 * undefined.
 */
void *bad_vec_pop(bad_vec_t *v);
char *bad_vec_elem_at(bad_vec_t *v, size_t i);
void bad_vec_map(bad_vec_t *v, void (*func) (void*));
/* TODO: Implement */
bad_vec_t *bad_vec_filter(bad_vec_t *v, bool (*func) (void*));
void bad_vec_fold(bad_vec_t *v, void *acc, void (*func)(void*, void*));
void bad_vec_sort(bad_vec_t *v);
bool bad_vec_is_strong(bad_vec_t *v);
size_t bad_vec_capacity(bad_vec_t *v);

char *bad_strcat(const char *dest, const char *src);
uint16_t num_digits(uintmax_t n);

