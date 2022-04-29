#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _list_node {
    void* data;
    void* data2;
    uint64_t data3;

    struct _list_node* next;
    struct _list_node* prev;
} list_node_t;

typedef struct _list {
    list_node_t* head;
} list_t;

typedef void(*list_clear_callback_t)(list_node_t*, void*);
typedef bool(*list_find_callback_t)(list_node_t*, void*);

int list_add(list_t* list, list_node_t* node);
int list_remove(list_t* list, list_node_t* node);
int list_clear(list_t* list, list_clear_callback_t callback, void* arg);
int list_find(list_t* list, list_find_callback_t callback, void* arg, list_node_t** nodeOut);

#ifdef __cplusplus
}
#endif
