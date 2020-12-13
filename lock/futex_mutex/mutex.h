#pragma once
#include <stdint.h>

typedef struct __mutex_t {
    int32_t* addr;
    uint32_t guard;
} mutex_t;

void mutex_init(mutex_t *lock);
void mutex_acquire(mutex_t *lock);
void mutex_release(mutex_t *lock);
