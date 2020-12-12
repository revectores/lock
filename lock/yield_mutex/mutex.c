#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include "mutex.h"

static inline uint xchg(volatile unsigned int *addr, unsigned int newval) {
    uint result;
    asm volatile("lock; xchgl %0, %1" : "+m" (*addr), "=a" (result) : "1" (newval) : "cc");
    return result;
}

void mutex_init(mutex_t *lock){
    lock->flag = 0;
}

void mutex_acquire(mutex_t *lock){
    while(xchg(&lock->flag, 1) == 1)
        sched_yield();
}

void mutex_release(mutex_t *lock){
    lock->flag = 0;
}
