#include <pthread.h>

typedef struct __node_t {
    unsigned int key;
    struct __node_t* next;
} node_t;

typedef struct __list_t {
    node_t* head;
    pthread_mutex_t lock;
} list_t;

void list_init(list_t *list);
void list_insert(list_t *list, unsigned int key);
void list_delete(list_t *list, unsigned int key);
void *list_lookup(list_t *list, unsigned int key);
