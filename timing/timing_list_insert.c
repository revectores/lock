#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <assert.h>

#include "list.h"
#include "timing.h"


list_t list;

void* thread_list_insert(void* arg){
    int insert_count = *(int*) arg;
    for (unsigned int t = 0; t < insert_count; t++){
        list_insert(&list, t);
    }
    return NULL;
}

void timing_list_insert(size_t thread_count, void* args){
    pthread_t threads[thread_count];
    size_t insert_count = *(int*)args;

    for (int i = 0; i < thread_count; i++){
        pthread_create(&threads[i], NULL, thread_list_insert, &insert_count);
    }

    for (int i = 0; i < thread_count; i++){
        pthread_join(threads[i], NULL);
    }
}

void validate_insertion(size_t thread_count, size_t insert_count){
    size_t* keycounts = (size_t*) malloc(insert_count * sizeof(size_t));
    node_t* node = list.head;
    while (node) {
        keycounts[node->key]++;
        node = node->next;
    }

    for (int i = 0; i < insert_count; i++){
        assert(keycounts[i] == thread_count);
    }
}

int main(int argc, char* argv[]){
    list_init(&list);
    if (argc != 3) {
        printf("Usage: ./timing_list_insert [thread_count] [insert_count]");
        return -1;
    }

    size_t thread_count = atoi(argv[1]);
    size_t insert_count = atoi(argv[2]);

    double elapsed = timeit(&timing_list_insert, thread_count, &insert_count);
    printf("%lf\n", elapsed);
    validate_insertion(thread_count, insert_count);

    return 0;
}