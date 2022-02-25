#include <stddef.h>
#include <malloc.h>

#include "list.h"


int list_add(list_t* list, list_node_t* node) {
    if (NULL != list->head) {
        list->head->prev = node;
    }

    node->next = list->head;
    node->prev = NULL;
    list->head = node;

    return 0;
}

int list_remove(list_t* list, list_node_t* node) {
    if (NULL != node->prev) {
        node->prev->next = node->next;
    }
    if (NULL != node->next) {
        node->next->prev = node->prev;
    }

    if (list->head == node) {
        list->head = node->next;
    }

    return 0;
}

int list_clear(list_t* list, list_clear_callback_t callback, void* arg) {
    list_node_t* node = list->head;
    while (NULL != node) {
        list_node_t* current = node;
        node = node->next;

        callback(current, arg);
        free(current);
    }

    list->head = NULL;

    return 0;
}

int list_find(list_t* list, list_find_callback_t callback, void* arg, list_node_t** nodeOut) {
    list_node_t* node = list->head;
    while (NULL != node) {
        if (callback(node, arg)) {
            *nodeOut = node;
            return 0;
        }

        node = node->next;
    }

    return 1;
}
