#pragma once

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void* ptr;
} hal_descriptor_element_t;

typedef struct {
    size_t capacity;
    hal_descriptor_element_t* elements;
} hal_descriptor_table_t;

int hal_descriptor_table_init(hal_descriptor_table_t* table, size_t size);
void hal_descriptor_table_free(hal_descriptor_table_t* table);
int hal_descriptor_table_add(hal_descriptor_table_t* table, void* ptr, size_t* index);
int hal_descriptor_table_insert(hal_descriptor_table_t* table, void* ptr, size_t index);
int hal_descriptor_table_remove(hal_descriptor_table_t* table, size_t index);
int hal_descriptor_table_get(const hal_descriptor_table_t* table, size_t index, void** ptr);
int hal_descriptor_table_find_next(const hal_descriptor_table_t* table, size_t start_index, size_t* index);

#ifdef __cplusplus
}
#endif
