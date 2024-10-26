#include <stdint.h>
#include <stdbool.h>

#define BAD_SIZE_NULL_TERMINATED ((size_t) 0u)

typedef struct bad_vec_t bad_vec_t;

bool bad_vec_init(
    bad_vec_t **v,
    size_t elem_size,
    void (*elem_destructor)(void*)
);
bool bad_vec_destroy(bad_vec_t **v);
void bad_vec_push(bad_vec_t *v, void *e);
char *bad_vec_elem_at(bad_vec_t *v, size_t i);
void bad_vec_foreach(bad_vec_t *v, void (*func) (void*));

char *bad_strcat(const char *dest, const char *src);
uint16_t num_digits(uintmax_t n);

