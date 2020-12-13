#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <linux/futex.h>
#include <time.h>
#include <sys/time.h>
#include <stdint.h>
#include <pthread.h>
#include "mutex.h"


static inline uint xchg(volatile unsigned int *addr, unsigned int newval) {
    uint result;
    asm volatile("lock; xchgl %0, %1" : "+m" (*addr), "=a" (result) : "1" (newval) : "cc");
    return result;
}

static long sys_futex(void *addr1, int op, int val1, struct timespec *timeout, void *addr2, int val3){
    return syscall(SYS_futex, addr1, op, val1, timeout, addr2, val3);
}

static long futex_wait(void *addr1, int val){
    return sys_futex(addr1, FUTEX_WAIT, val, NULL, NULL, 0);
}

static long futex_wake(void *addr1){
    return sys_futex(addr1, FUTEX_WAKE, 1, NULL, NULL, 0);
}

int atomic_bit_test_set(mutex_t *lock){
    while (xchg(&lock->guard, 1) == 1);
    int32_t new = *(lock->addr) | 0x80000000;
    int32_t v = xchg(lock->addr, new);
    int32_t is_blocked = (v < 0);
    lock->guard = 0;
    return is_blocked;
}

void atomic_decrement(mutex_t *lock){
    while (xchg(&lock->guard, 1) == 1);
    int new = *(lock->addr) - 1;
    xchg(lock->addr, new);
    lock->guard = 0;
}

void atomic_increment(mutex_t *lock){
    while (xchg(&lock->guard, 1) == 1);
    int new = *(lock->addr) + 1;
    xchg(lock->addr, new);
    lock->guard = 0;
}

int atomic_add(mutex_t *lock){
    while (xchg(&lock->guard, 1) == 1);
    int sum = *(lock->addr) ^ 0x80000000;
    xchg(lock->addr, sum);
    lock->guard = 0;
    return sum;
}

void mutex_init(mutex_t *lock){
    lock->addr = malloc(sizeof(uint));
    lock->guard = 0;
}

void mutex_acquire(mutex_t *lock){

    if (atomic_bit_test_set(lock) == 0){
        return;
    }

    atomic_increment(lock);

    while (1){
        if (atomic_bit_test_set(lock) == 0){
            atomic_decrement(lock);
            return;
        }

        uint value = *(lock->addr);
        if (value >= 0)
            continue;
        futex_wait(lock->addr, value);
    }
}

void mutex_release(mutex_t *lock){
    if (!atomic_add(lock))
        return;

    futex_wake(lock->addr);
}
