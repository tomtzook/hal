
#include "logging/log.h"

#include "port_table.h"
#include "hal_global.h"

static int port_id_compare(port_id_t first, port_id_t second) {
    return first == second;
}

hal_result_t table_find_port(lookup_table_t* table, port_id_t port_id, hal_handle_t* result_handle) {
    generic_port_t* port;
    lookup_table_index_t index;

    for (index = 0; index < table->capacity; ++index) {
        lookup_table_result_t result = lookup_table_get(table, index, (void **) &port);

        if (LOOKUP_TABLE_SUCCESS == result && port_id_compare(port->port_id, port_id)) {
            *result_handle = (hal_handle_t) index;
            return HAL_SUCCESS;
        }
    }

    *result_handle =  HAL_INVALID_HANDLE;
    return HAL_PORT_NOT_FOUND;
}
hal_result_t table_get_port(lookup_table_t* table, hal_handle_t hal_handle, void** result_port_struct) {
    lookup_table_result_t result = lookup_table_get(table, (lookup_table_index_t) hal_handle,
            result_port_struct);

    if (result != LOOKUP_TABLE_SUCCESS) {
        LOGLN("failed to get port from table: %d", result);
        return HAL_PORT_NOT_FOUND;
    }

    return HAL_SUCCESS;
}

hal_result_t table_insert_port(lookup_table_t* table, void* port_struct, hal_handle_t* result_handle) {
    lookup_table_result_t result = lookup_table_insert(table, port_struct,
                                                       (lookup_table_index_t*) result_handle);

    if (LOOKUP_TABLE_SUCCESS != result) {
        LOGLN("failed to insert port into table: %d", result);
        return HAL_STORE_ERROR;
    }

    return HAL_SUCCESS;
}
hal_result_t table_remove_port(lookup_table_t* table, hal_handle_t hal_handle) {
    lookup_table_result_t result = lookup_table_remove(table, (lookup_table_index_t) hal_handle);

    if (LOOKUP_TABLE_SUCCESS != result) {
        LOGLN("failed to remove port from table: %d", result);
        return HAL_STORE_ERROR;
    }

    return HAL_SUCCESS;
}