#include <stdio.h>
#include <pthread.h>
#include "counter.h"

counter_t counter;

void* incr_counter(void* arg){
	int incr_c = *(int*)arg;
	for (int t = 0; t < incr_c; t++){
		counter_increment(&counter);
	}
	return NULL;
}

int main(int argc, char* argv[]){
	counter_init(&counter, 0);
	int thread_count = 10;
	int incr_time = 1000;

	pthread_t threads[thread_count];
	for (int i = 0; i < thread_count; i++){
		pthread_create(&threads[i], NULL, incr_counter, &incr_time);
	}

	for (int i = 0; i < thread_count; i++){
		pthread_join(threads[i], NULL);
	}

	int v = counter_get_value(&counter);
	printf("%d\n", v);

	return 0;
}