#include <stdlib.h>
#include "hash.h"


void hash_init(hash_t *hash, int size){
    hash->size = size;
    hash->lists = (list_t*) malloc(size * sizeof(list_t));

    for (int i = 0; i < size; i++){
        list_init(&hash->lists[i]);
    }

    spinlock_init(&hash->lock);
}

void hash_insert(hash_t *hash, unsigned int key){
    unsigned int entry = key % (hash->size);
    list_insert(&hash->lists[entry], key);
}

void hash_delete(hash_t *hash, unsigned int key){
    unsigned int entry = key % (hash->size);
    list_delete(&hash->lists[entry], key);
}

void* hash_lookup(hash_t *hash, unsigned int key){
    unsigned int entry = key % (hash->size);
    return list_lookup(&hash->lists[entry], key);
}