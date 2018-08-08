#ifndef HAL_LOOKUP_TABLE_H
#define HAL_LOOKUP_TABLE_H

#include <wchar.h>

typedef size_t lookup_table_index_t;

typedef struct {
    void** table;
    size_t capacity;
} lookup_table_t;

typedef enum {
    LOOKUP_TABLE_SUCCESS = 0,
    LOOKUP_TABLE_ALREADY_INITIALIZED = 1,
    LOOKUP_TABLE_NOT_INITIALIZED = 2,
    LOOKUP_TABLE_MEMORY_ALLOCATION_ERROR = 3,
    LOOKUP_TABLE_INDEX_OUT_OF_BOUNDS = 4,
    LOOKUP_TABLE_RESIZE_ERROR = 5
} lookup_table_result_t;

lookup_table_result_t lookup_table_init(lookup_table_t* table);
void lookup_table_free(lookup_table_t* table);

lookup_table_result_t lookup_table_insert(lookup_table_t* table, void* obj, lookup_table_index_t* result_index);
lookup_table_result_t lookup_table_remove(lookup_table_t* table, lookup_table_index_t pos);
lookup_table_result_t lookup_table_get(lookup_table_t* table, lookup_table_index_t pos, void** result);

#endif //HAL_LOOKUP_TABLE_H
