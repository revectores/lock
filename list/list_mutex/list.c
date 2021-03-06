#include <stdlib.h>
#include "list.h"


void list_init(list_t *list){
    list->head = NULL;
    mutex_init(&list->lock);
};

void list_insert(list_t *list, unsigned int key){
    mutex_acquire(&list->lock);

    node_t* node = (node_t*) malloc(sizeof(node_t));
    node->key = key;

    if (list->head == NULL){
        node->next = NULL;
        list->head = node;
    } else {
        node->next = list->head;
        list->head = node;
    }

    mutex_release(&list->lock);
};

void list_delete(list_t *list, unsigned int key){
    mutex_acquire(&list->lock);

    node_t* node = list->head;
    node_t* prev = NULL;

    while (node){
        if (node->key == key){
            if (prev == NULL) {
                list->head = node->next;
            } else {
                prev->next = node->next;
            }
        }
        prev = node;
    }

    mutex_release(&list->lock);
};

void *list_lookup(list_t *list, unsigned int key){
    mutex_acquire(&list->lock);

    node_t* node = list->head;
    while (node){
        if (node->key == key){
            mutex_release(&list->lock);
            return node;
        }
    }

    mutex_release(&list->lock);
    return NULL;
};
