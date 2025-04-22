#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DECLARE_HAL_LIST(data_type, name) \
    typedef struct _hal_list_ ##name { \
        size_t size; \
        size_t capacity; \
        data_type * elements; \
    } hal_list_ ##name## _t; \
    int hal_list_ ##name## _init(hal_list_ ##name## _t* list, size_t initial_capacity); \
    void hal_list_ ##name## _free(hal_list_ ##name## _t* list); \
    int hal_list_ ##name## _add(hal_list_ ##name## _t* list, data_type data, size_t* index); \
    int hal_list_ ##name## _get(hal_list_ ##name## _t* list, size_t index, data_type * data); \

#define DEFINE_HAL_LIST(data_type, name) \
    int hal_list_ ##name## _init(hal_list_ ##name## _t* list, size_t initial_capacity) { \
        const size_t mem_size = sizeof( data_type ) * initial_capacity;  \
        data_type * elements = malloc(mem_size);                         \
        if (elements == NULL) {                                              \
            return HAL_BASIC_ERROR;                                          \
        }                                                                    \
                                                                             \
        memset(elements, 0, mem_size);                                       \
        list->elements = elements;                                           \
        list->size = 0;                                                      \
        list->capacity = initial_capacity;                                   \
        return HAL_BASIC_SUCCESS;                                            \
    } \
    void hal_list_ ##name## _free(hal_list_ ##name## _t* list) { \
        if (list->elements == NULL) {                                         \
            return;                                                           \
        }                                                                     \
        free(list->elements);                                                 \
        list->elements = NULL;                                                \
    }\
    int hal_list_ ##name## _add(hal_list_ ##name## _t* list, data_type data, size_t* index) { \
        if (list->elements == NULL) {                                         \
            return;                                                           \
        }                                                                      \
        if (list->size >= list->capacity) {                                     \
            const size_t new_capacity = list->capacity + 10;                    \
            const size_t memsize = new_capacity * sizeof( data_type );      \
            data_type * new_elements = realloc(list->elements, memsize);    \
            if (new_elements == NULL) {                                         \
                return HAL_BASIC_ERROR;                                         \
            }                                                                   \
            list->elements = new_elements;                                      \
            list->capacity = new_capacity;                                      \
        }                                                                     \
                                                                              \
        const size_t used_index = list->size++;                               \
        list->elements[used_index] = data;                                    \
        *index = used_index;                                                  \
        return HAL_BASIC_SUCCESS;                                             \
    }\
    int hal_list_ ##name## _get(hal_list_ ##name## _t* list, size_t index, data_type * data) { \
        if (index >= list->size) {                              \
            return HAL_BASIC_ERROR;                             \
        }                                                       \
        *data = list->elements[index];                          \
        return HAL_BASIC_SUCCESS;                               \
    }\


typedef struct _hal_linked_list_node {
    struct _hal_linked_list_node* next;
    struct _hal_linked_list_node* prev;
    uint8_t data[];
} hal_linked_list_node_t;

typedef struct _hal_linked_list {
    struct _hal_linked_list_node* head;
} hal_linked_list_t;

int hal_linked_list_add(hal_linked_list_t* list, hal_linked_list_node_t* node);
int hal_linked_list_remove(hal_linked_list_t* list, const hal_linked_list_node_t* node);

DECLARE_HAL_LIST(uint32_t, uint32);

#ifdef __cplusplus
}
#endif
