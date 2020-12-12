#pragma once

typedef unsigned int uint;

typedef struct __mutex_t {
	uint flag;
} mutex_t;

void mutex_init(mutex_t *lock);
void mutex_acquire(mutex_t *lock);
void mutex_release(mutex_t *lock);
