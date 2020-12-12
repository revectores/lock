#include "spinlock.h"
#include "list.h"


typedef struct __hash_t {
    unsigned int size;
    list_t* lists;
    spinlock_t lock;
} hash_t;

void hash_init(hash_t *hash, int size);
void hash_insert(hash_t *hash, unsigned int key);
void hash_delete(hash_t *hash, unsigned int key);
void *hash_lookup(hash_t *hash, unsigned int key);