#include <stdint.h>
#include <stdbool.h>

#define BAD_SIZE_NULL_TERMINATED ((size_t) 0u)

/**
 * A stack-line variable length array that allows for map, filter and fold
 * operations. Elements placed into the vector are copied and the vector
 * retains ownership of them until/if they are explicitly popped or removed.
 *
 * This structure can only store types with a fixed size or null-terminated
 * strings. To see how these cases are specified, take a look at bad_vec_init
 * and BAD_SIZE_NULL_TERMINATED.
 */
typedef struct bad_vec_t bad_vec_t;

bool bad_vec_init(bad_vec_t **v, size_t elem_size);
bool bad_vec_destroy(bad_vec_t **v);
void bad_vec_push(bad_vec_t *v, void *e);
/* TODO: Implement */
void *bad_vec_pop(bad_vec_t *v);
char *bad_vec_elem_at(bad_vec_t *v, size_t i);
void bad_vec_map(bad_vec_t *v, void (*func) (void*));
/* TODO: Implement */
bad_vec_t *bad_vec_filter(bad_vec_t *v, bool (*func) (void*));
void bad_vec_fold(bad_vec_t *v, void *acc, void (*func)(void*, void*));

char *bad_strcat(const char *dest, const char *src);
uint16_t num_digits(uintmax_t n);

