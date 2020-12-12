#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <assert.h>

#include "hash.h"
#include "timing.h"


hash_t hash;

void* thread_hash_insert(void* args){
    int insert_count = *(int*) args;
    for (unsigned int t = 0; t < insert_count; t++){
        hash_insert(&hash, t);
    }
    return NULL;
}

void timing_hash_insert(size_t thread_count, void* args){
    pthread_t threads[thread_count];
    size_t insert_count = *(int*)args;

    for (int i = 0; i < thread_count; i++){
        pthread_create(&threads[i], NULL, thread_hash_insert, &insert_count);
    }

    for (int i = 0; i < thread_count; i++){
        pthread_join(threads[i], NULL);
    }
}

void validate_insertion(size_t thread_count, size_t bucket_count, size_t insert_count){
    size_t* keycounts = (size_t*) malloc(insert_count * sizeof(size_t));

    for (int k = 0; k < hash.size; k++){
        node_t* node = hash.lists[k].head;

        while (node) {
            keycounts[node->key]++;
            node = node->next;
        }
    }
    for (int i = 0; i < insert_count; i++){
        assert(keycounts[i] == thread_count);
    }
}

int main(int argc, char* argv[]){
    if (argc != 4){
        printf("Usage: ./timing_hash_insert [thread_count] [bucket_count] [insert_count]");
    }

    size_t thread_count = atoi(argv[1]);
    size_t bucket_count = atoi(argv[2]);
    size_t insert_count = atoi(argv[3]);

    hash_init(&hash, bucket_count);

    double elapsed = timeit(&timing_hash_insert, thread_count, &insert_count);
    printf("%lf\n", elapsed);
    validate_insertion(thread_count, bucket_count, insert_count);

    return 0;
}