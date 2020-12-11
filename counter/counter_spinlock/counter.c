#include "counter.h"


void counter_init(counter_t *c, int value){
    spinlock_init(&c->lock);

    spinlock_acquire(&c->lock);
    c->value = value;
    spinlock_release(&c->lock);
}

int counter_get_value(counter_t *c){
    return c->value;
}

void counter_increment(counter_t *c){
    spinlock_acquire(&c->lock);
    c->value++;
    spinlock_release(&c->lock);
}

void counter_decrement(counter_t *c){
    spinlock_acquire(&c->lock);
    c->value--;
    spinlock_release(&c->lock);
};
