#include <stdlib.h>
#include <string.h>

#include "lookup_table.h"


#define DEFAULT_CAPACITY (20)

#define NEW_CAPACITY(old) (old * 2)

#define CHECK_INITIALIZED(table) \
    if (table->table == NULL) { \
        return LOOKUP_TABLE_NOT_INITIALIZED; \
    }

#define CHECK_INDEX(table, idx) \
    if (idx >= table->capacity) { \
        return LOOKUP_TABLE_INDEX_OUT_OF_BOUNDS; \
    }


static lookup_table_index_t get_next_free_index(lookup_table_t* table);
static lookup_table_result_t increase_table_capacity(lookup_table_t* table);


lookup_table_result_t
lookup_table_init(lookup_table_t* table) {
    if (NULL != table->table) {
        return LOOKUP_TABLE_ALREADY_INITIALIZED;
    }

    void** table_ptr = malloc(sizeof(void*) * DEFAULT_CAPACITY);
    if (table_ptr == NULL) {
        return LOOKUP_TABLE_MEMORY_ALLOCATION_ERROR;
    }

    memset(table_ptr, 0, DEFAULT_CAPACITY * sizeof(void*));

    table->table = table_ptr;
    table->capacity = DEFAULT_CAPACITY;

    return LOOKUP_TABLE_SUCCESS;
}

void
lookup_table_free(lookup_table_t* table) {
    if (table->table == NULL) {
        return;
    }

    free(table->table);

    table->table = NULL;
    table->capacity = 0;
}

lookup_table_result_t
lookup_table_insert(lookup_table_t* table, void* obj, lookup_table_index_t* result_index) {
    CHECK_INITIALIZED(table);

    lookup_table_index_t insert_index = get_next_free_index(table);

    if (insert_index >= table->capacity) {
        lookup_table_result_t increase_result = increase_table_capacity(table);

        if (increase_result != LOOKUP_TABLE_SUCCESS) {
            return LOOKUP_TABLE_RESIZE_ERROR;
        }
    }

    table->table[insert_index] = obj;
    *result_index = insert_index;

    return LOOKUP_TABLE_SUCCESS;
}

lookup_table_result_t
lookup_table_remove(lookup_table_t* table, lookup_table_index_t pos) {
    CHECK_INITIALIZED(table);
    CHECK_INDEX(table, pos);

    table->table[pos] = NULL;
    return LOOKUP_TABLE_SUCCESS;
}

lookup_table_result_t
lookup_table_get(lookup_table_t* table, lookup_table_index_t pos, void** result) {
    CHECK_INITIALIZED(table);
    CHECK_INDEX(table, pos);

    void* entry = table->table[pos];
    if (NULL == entry) {
        return LOOKUP_TABLE_ENTRY_EMPTY;
    }

    *result = entry;
    return LOOKUP_TABLE_SUCCESS;
}



lookup_table_index_t
get_next_free_index(lookup_table_t* table) {
    lookup_table_index_t idx;

    for (idx = 0; idx < table->capacity; ++idx) {
        if (NULL == table->table[idx]) {
            break;
        }
    }

    return idx;
}

lookup_table_result_t
increase_table_capacity(lookup_table_t* table) {
    lookup_table_index_t new_capacity = NEW_CAPACITY(table->capacity);

    void** new_table = malloc(sizeof(void*) * new_capacity);
    if (new_table == NULL) {
        return LOOKUP_TABLE_MEMORY_ALLOCATION_ERROR;
    }

    memcpy(new_table, table->table, table->capacity);
    free(table->table);

    memset(new_table, table->capacity * sizeof(void*), new_capacity * sizeof(void*));

    table->table = new_table;
    table->capacity = new_capacity;

    return LOOKUP_TABLE_SUCCESS;
}
