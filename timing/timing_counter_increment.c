#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <assert.h>

#include "counter.h"
#include "timing.h"


counter_t counter;

void* thread_counter_increment(void* args){
    int incr_c = *(int*)args;
    for (int t = 0; t < incr_c; t++){
        counter_increment(&counter);
    }
    return NULL;
}

void timing_counter_increment(size_t thread_count, void* args){
    pthread_t threads[thread_count];
    size_t incr_count = *(int*)args;

    for (int i = 0; i < thread_count; i++){
        pthread_create(&threads[i], NULL, thread_counter_increment, &incr_count);
    }

    for (int i = 0; i < thread_count; i++){
        pthread_join(threads[i], NULL);
    }
}

void validate_increment(size_t thread_count, size_t incr_count){
    int counter_value = counter_get_value(&counter);
    assert(counter_value == thread_count * incr_count);
}


int main(int argc, char* argv[]){
    if (argc != 3) {
        printf("Usage: ./timing_counter_increment [thread_count] [incr_count]");
        return -1;
    }

    size_t thread_count = atoi(argv[1]);
    size_t incr_count = atoi(argv[2]);

    counter_init(&counter, 0);

    double elapsed = timeit(&timing_counter_increment, thread_count, &incr_count);
    printf("%lf\n", elapsed);
    validate_increment(thread_count, incr_count);

    return 0;
}