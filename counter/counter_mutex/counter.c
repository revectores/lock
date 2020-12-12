#include "counter.h"


void counter_init(counter_t *c, int value){
    mutex_init(&c->lock);

    mutex_acquire(&c->lock);
    c->value = value;
    mutex_release(&c->lock);
}

int counter_get_value(counter_t *c){
    return c->value;
}

void counter_increment(counter_t *c){
    mutex_acquire(&c->lock);
    c->value++;
    mutex_release(&c->lock);
}

void counter_decrement(counter_t *c){
    mutex_acquire(&c->lock);
    c->value--;
    mutex_release(&c->lock);
};
