#include <hal_dio.h>
#include <dio_interface.h>
#include <lookup_table.h>
#include <stdlib.h>

#include "hal_global.h"

typedef struct {
    port_id_t port_id;
    port_dir_t port_dir;

    dio_value_t last_value;
} dio_port_t;


static hal_handle_t find_port_in_table(port_id_t port_id);
static dio_port_t* get_port_from_table(hal_handle_t hal_handle);
static hal_handle_t insert_port_to_table(dio_port_t* port);
static int remove_port_from_table(hal_handle_t hal_handle);
static int port_id_compare(port_id_t first, port_id_t second);

/*********************************/
/*             EXTERNAL          */
/*********************************/

lookup_table_t dio_table;


hal_result_t hal_dio_init_module() {
    if (HAL_NOT_INITIALIZED()) {
        return HAL_NOT_INITIALIZED;
    }

    lookup_table_result_t table_init_result = lookup_table_init(&dio_table);
    if (table_init_result != LOOKUP_TABLE_SUCCESS) {
        return HAL_INITIALIZATION_ERROR;
    }

    if (!dio_init()) {
        lookup_table_free(&dio_table);
        return HAL_IO_ERROR;
    }

    return HAL_SUCCESS;
}

void hal_dio_free_module() {
    if (HAL_NOT_INITIALIZED()) {
        return;
    }

    lookup_table_free(&dio_table);
    dio_free();
}

hal_result_t hal_dio_init(port_id_t port_id, port_dir_t port_dir, hal_handle_t* result) {
    if (HAL_NOT_INITIALIZED()) {
        return HAL_NOT_INITIALIZED;
    }

    dio_port_t* port;
    hal_handle_t port_handle = find_port_in_table(port_id);

    if (port_handle == HAL_INVALID_HANDLE) {
        port = malloc(sizeof(dio_port_t));
        if (port == NULL) {
            return HAL_MEMORY_ALLOCATION_ERROR;
        }

        port_handle = insert_port_to_table(port);
        if (port_handle == HAL_INVALID_HANDLE) {
            free(port);

            return HAL_STORE_ERROR;
        }

        if (!dio_port_init(port_id, port_dir)) {
            remove_port_from_table(port_handle);
            free(port);

            return HAL_IO_ERROR;
        }
    } else {
        port = get_port_from_table(port_handle);

        if (port_dir != port->port_dir) {
            return HAL_ARGUMENT_ERROR;
        }
    }

    *result = port_handle;
    return HAL_SUCCESS;
}

hal_result_t hal_dio_free(hal_handle_t hal_handle) {
    if (HAL_NOT_INITIALIZED()) {
        return HAL_NOT_INITIALIZED;
    }

    dio_port_t* port = get_port_from_table(hal_handle);

    if (port == NULL) {
        return HAL_PORT_NOT_INITIALIZED;
    }

    if (!remove_port_from_table(hal_handle)) {
        return HAL_STORE_ERROR;
    }

    port_id_t port_id = port->port_id;

    free(port);
    dio_port_free(port_id);

    return HAL_SUCCESS;
}

hal_result_t hal_dio_set(hal_handle_t hal_handle, dio_value_t dio_value) {
    if (HAL_NOT_INITIALIZED()) {
        return HAL_NOT_INITIALIZED;
    }

    dio_port_t* port = get_port_from_table(hal_handle);

    if (port == NULL) {
        return HAL_PORT_NOT_INITIALIZED;
    }

    if (port->port_dir != OUTPUT) {
        return HAL_ARGUMENT_ERROR;
    }

    dio_port_write(port->port_id, dio_value);
    port->last_value = dio_value;

    return HAL_SUCCESS;
}

hal_result_t hal_dio_get(hal_handle_t hal_handle, dio_value_t* result) {
    if (HAL_NOT_INITIALIZED()) {
        return HAL_NOT_INITIALIZED;
    }

    dio_port_t* port = get_port_from_table(hal_handle);

    if (port == NULL) {
        return HAL_PORT_NOT_INITIALIZED;
    }

    if (port->port_dir == OUTPUT) {
        *result = port->last_value;
    } else {
        *result = dio_port_read(port->port_id);
    }

    return HAL_SUCCESS;
}

/*********************************/
/*             STATIC            */
/*********************************/

hal_handle_t find_port_in_table(port_id_t port_id) {
    dio_port_t* port;
    lookup_table_index_t index;

    for (index = 0; index < dio_table.capacity; ++index) {
        lookup_table_result_t result = lookup_table_get(&dio_table, index, (void **) &port);
        if (result == LOOKUP_TABLE_SUCCESS && port_id_compare(port->port_id, port_id)) {
            return (hal_handle_t) index;
        }
    }

    return HAL_INVALID_HANDLE;
}

dio_port_t* get_port_from_table(hal_handle_t hal_handle) {
    dio_port_t* port;
    lookup_table_result_t result = lookup_table_get(&dio_table, (lookup_table_index_t) hal_handle, (void**) &port);

    if (result != LOOKUP_TABLE_SUCCESS) {
        return NULL;
    }

    return port;
}

hal_handle_t insert_port_to_table(dio_port_t* port) {
    hal_handle_t hal_handle;

    lookup_table_result_t result = lookup_table_insert(&dio_table, port, (lookup_table_index_t*) &hal_handle);

    if (result != LOOKUP_TABLE_SUCCESS) {
        return HAL_INVALID_HANDLE;
    }

    return hal_handle;
}

int remove_port_from_table(hal_handle_t hal_handle) {
    lookup_table_result_t result = lookup_table_remove(&dio_table, (lookup_table_index_t) hal_handle);

    if (result != LOOKUP_TABLE_SUCCESS) {
        return 0;
    }

    return 1;
}

int port_id_compare(port_id_t first, port_id_t second) {
    return first.connection == second.connection;
}
