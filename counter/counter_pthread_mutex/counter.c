#include "counter.h"


void counter_init(counter_t *c, int value){
    pthread_mutex_init(&c->lock, NULL);

    pthread_mutex_lock(&c->lock);
    c->value = value;
    pthread_mutex_unlock(&c->lock);
}

int counter_get_value(counter_t *c){
    return c->value;
}

void counter_increment(counter_t *c){
    pthread_mutex_lock(&c->lock);
    c->value++;
    pthread_mutex_unlock(&c->lock);
}

void counter_decrement(counter_t *c){
    pthread_mutex_lock(&c->lock);
    c->value--;
    pthread_mutex_unlock(&c->lock);
};
