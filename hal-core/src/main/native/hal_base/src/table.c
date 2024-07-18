
#include <malloc.h>
#include <string.h>

#include "hal_internal.h"
#include "hal_table.h"

static int find_next_free_index(hal_descriptor_table_t* table, size_t* index) {
    size_t _index = 0;
    for (; _index < table->capacity; _index++) {
        if (table->elements[_index].ptr == NULL) {
            *index = _index;
            return HAL_BASIC_SUCCESS;
        }
    }

    *index = 0;
    return HAL_BASIC_ERROR;
}

int hal_descriptor_table_init(hal_descriptor_table_t* table, size_t size) {
    size_t mem_size = sizeof(hal_descriptor_element_t) * size;
    hal_descriptor_element_t* elements = malloc(mem_size);
    if (elements == NULL) {
        return HAL_BASIC_ERROR;
    }

    memset(elements, 0, mem_size);
    table->elements = elements;
    table->capacity = size;

    return HAL_BASIC_SUCCESS;
}

void hal_descriptor_table_free(hal_descriptor_table_t* table) {
    if (table->elements == NULL) {
        return;
    }

    free(table->elements);
}

int hal_descriptor_table_add(hal_descriptor_table_t* table, void* ptr, size_t* index) {
    if (ptr == NULL) {
        return HAL_BASIC_ERROR;
    }

    size_t _index;
    if (find_next_free_index(table, &_index)) {
        return HAL_BASIC_ERROR;
    }

    table->elements[_index].ptr = ptr;

    *index = _index;
    return HAL_BASIC_SUCCESS;
}

int hal_descriptor_table_insert(hal_descriptor_table_t* table, void* ptr, size_t index) {
    if (ptr == NULL) {
        return HAL_BASIC_ERROR;
    }

    if (table->elements[index].ptr != NULL) {
        return HAL_BASIC_ERROR;
    }

    table->elements[index].ptr = ptr;

    return HAL_BASIC_SUCCESS;
}

int hal_descriptor_table_remove(hal_descriptor_table_t* table, size_t index) {
    if (index >= table->capacity) {
        return HAL_BASIC_ERROR;
    }

    table->elements[index].ptr = NULL;
    return HAL_BASIC_SUCCESS;
}

int hal_descriptor_table_get(const hal_descriptor_table_t* table, size_t index, void** ptr) {
    if (index >= table->capacity) {
        return HAL_BASIC_ERROR;
    }

    void* _ptr = table->elements[index].ptr;
    if (_ptr == NULL) {
        return HAL_BASIC_ERROR;
    }

    *ptr = _ptr;
    return HAL_BASIC_SUCCESS;
}

int hal_descriptor_table_find_next(const hal_descriptor_table_t* table, size_t start_index, size_t* index) {
    for (size_t i = start_index; i < table->capacity; ++i) {
        void* ptr;
        if (!hal_descriptor_table_get(table, i, &ptr)) {
            // found
            *index = i;
            return HAL_BASIC_SUCCESS;
        }
    }

    return HAL_BASIC_ERROR;
}
