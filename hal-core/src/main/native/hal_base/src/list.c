#include <stddef.h>

#include "hal_internal.h"
#include "hal_list.h"


int hal_list_add(hal_list_t* list, hal_list_node_t* node) {
    if (NULL != list->head) {
        list->head->prev = node;
    }

    node->next = list->head;
    node->prev = NULL;
    list->head = node;

    return HAL_BASIC_SUCCESS;
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

    return HAL_BASIC_SUCCESS;
}
