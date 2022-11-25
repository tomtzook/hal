#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _hal_list_node {
    struct _hal_list_node* next;
    struct _hal_list_node* prev;
    uint8_t data[];
} hal_list_node_t;

typedef struct _hal_list {
    struct _hal_list_node* head;
} hal_list_t;

int hal_list_add(hal_list_t* list, hal_list_node_t* node);
int hal_list_remove(hal_list_t* list, hal_list_node_t* node);

#ifdef __cplusplus
}
#endif
