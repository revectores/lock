#pragma once

typedef unsigned int uint;

typedef struct __spinlock_t {
    uint flag;
} spinlock_t;

void spinlock_init(spinlock_t *lock);
void spinlock_acquire(spinlock_t *lock);
void spinlock_release(spinlock_t *lock);
