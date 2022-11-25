#include <stddef.h>

#include "list.h"


int hal_list_add(hal_list_t* list, hal_list_node_t* node) {
    if (NULL != list->head) {
        list->head->prev = node;
    }

    node->next = list->head;
    node->prev = NULL;
    list->head = node;

    return 0;
}

int hal_list_remove(hal_list_t* list, hal_list_node_t* node) {
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
