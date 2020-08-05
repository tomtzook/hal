#include <stdlib.h>

#include "hal_dio.h"
#include "hal_global.h"
#include "logging/log.h"
#include "interface/dio_interface.h"

#include "hal_modules.h"

#define SUCCESS (0)

typedef struct {
    port_id_t port_id;
    port_dir_t port_dir;

    dio_value_t last_value;
} dio_port_t;


static hal_handle_t find_port_in_table(hal_env_t* env, port_id_t port_id);
static dio_port_t* get_port_from_table(hal_env_t* env, hal_handle_t hal_handle);
static hal_handle_t insert_port_to_table(hal_env_t* env, dio_port_t* port);
static int remove_port_from_table(hal_env_t* env, hal_handle_t hal_handle);
static int port_id_compare(port_id_t first, port_id_t second);

/*********************************/
/*             EXTERNAL          */
/*********************************/


hal_result_t hal_dio_init_module(hal_env_t* env) {
    lookup_table_result_t table_init_result = lookup_table_init(&env->dio_table);
    if (LOOKUP_TABLE_SUCCESS != table_init_result) {
        LOGLN("failed to initialize lookup table: %d", table_init_result);
        return HAL_INITIALIZATION_ERROR;
    }

    return HAL_SUCCESS;
}

void hal_dio_free_module(hal_env_t* env) {
    if (HAL_NOT_INITIALIZED(env)) {
        return;
    }

    lookup_table_free(&env->dio_table);
}

hal_result_t hal_dio_init(hal_env_t* env, port_id_t port_id, port_dir_t port_dir, hal_handle_t* result_handle) {
    if (HAL_NOT_INITIALIZED(env)) {
        return HAL_NOT_INITIALIZED;
    }

    hal_result_t result;
    dio_port_t* port;
    hal_handle_t port_handle = find_port_in_table(env, port_id);

    if (HAL_INVALID_HANDLE == port_handle) {
        port = malloc(sizeof(dio_port_t));
        if (NULL == port) {
            return HAL_MEMORY_ALLOCATION_ERROR;
        }

        port->port_dir = port_dir;
        port->port_id = port_id;
        port->last_value = DIO_VALUE_LOW;

        port_handle = insert_port_to_table(env, port);
        if (HAL_INVALID_HANDLE == port_handle) {
            free(port);

            return HAL_STORE_ERROR;
        }

        result = dio_port_init(env->interface_env, port_id, port_dir);
        if (HAL_SUCCESS != result) {
            remove_port_from_table(env, port_handle);
            free(port);

            return result;
        }
    } else {
        port = get_port_from_table(env, port_handle);

        if (port_dir != port->port_dir) {
            return HAL_ARGUMENT_ERROR;
        }
    }

    *result_handle = port_handle;
    return HAL_SUCCESS;
}

hal_result_t hal_dio_free(hal_env_t* env, hal_handle_t hal_handle) {
    if (HAL_NOT_INITIALIZED(env)) {
        return HAL_NOT_INITIALIZED;
    }

    dio_port_t* port = get_port_from_table(env, hal_handle);
    if (NULL == port) {
        return HAL_PORT_NOT_INITIALIZED;
    }

    if (!remove_port_from_table(env, hal_handle)) {
        return HAL_STORE_ERROR;
    }

    port_id_t port_id = port->port_id;

    free(port);
    dio_port_free(env->interface_env, port_id);

    return HAL_SUCCESS;
}

hal_result_t hal_dio_set(hal_env_t* env, hal_handle_t hal_handle, dio_value_t dio_value) {
    if (HAL_NOT_INITIALIZED(env)) {
        return HAL_NOT_INITIALIZED;
    }

    hal_result_t result;

    dio_port_t* port = get_port_from_table(env, hal_handle);
    if (NULL == port) {
        return HAL_PORT_NOT_INITIALIZED;
    }

    if (PORT_DIR_OUTPUT != port->port_dir) {
        return HAL_ARGUMENT_ERROR;
    }

    result = dio_port_write(env->interface_env, port->port_id, dio_value);
    if (HAL_SUCCESS != result) {
        return result;
    }

    port->last_value = dio_value;

    return HAL_SUCCESS;
}

hal_result_t hal_dio_get(hal_env_t* env, hal_handle_t hal_handle, dio_value_t* result_value) {
    if (HAL_NOT_INITIALIZED(env)) {
        return HAL_NOT_INITIALIZED;
    }

    hal_result_t result;

    dio_port_t* port = get_port_from_table(env, hal_handle);
    if (NULL == port) {
        return HAL_PORT_NOT_INITIALIZED;
    }

    if (port->port_dir == PORT_DIR_OUTPUT) {
        *result_value = port->last_value;
    } else {
        result = dio_port_read(env->interface_env, port->port_id, result_value);
        if (result) {
            return result;
        }
    }

    return HAL_SUCCESS;
}

hal_result_t hal_dio_pulse(hal_env_t* env, hal_handle_t hal_handle, uint64_t length_us) {
    if (HAL_NOT_INITIALIZED(env)) {
        return HAL_NOT_INITIALIZED;
    }

    hal_result_t result;

    dio_port_t* port = get_port_from_table(env, hal_handle);
    if (port == NULL) {
        return HAL_PORT_NOT_INITIALIZED;
    }

    if (PORT_DIR_OUTPUT != port->port_dir) {
        return HAL_ARGUMENT_ERROR;
    }

    result = dio_port_pulse(env->interface_env, port->port_id, length_us);
    if (result) {
        return result;
    }

    return HAL_SUCCESS;
}

/*********************************/
/*             STATIC            */
/*********************************/

hal_handle_t find_port_in_table(hal_env_t* env, port_id_t port_id) {
    dio_port_t* port;
    lookup_table_t* dio_table = &(env->dio_table);
    lookup_table_index_t index;

    for (index = 0; index < env->dio_table.capacity; ++index) {
        lookup_table_result_t result = lookup_table_get(dio_table, index, (void **) &port);

        if (LOOKUP_TABLE_SUCCESS == result && port_id_compare(port->port_id, port_id)) {
            return (hal_handle_t) index;
        }
    }

    return HAL_INVALID_HANDLE;
}

dio_port_t* get_port_from_table(hal_env_t* env, hal_handle_t hal_handle) {
    dio_port_t* port;
    lookup_table_result_t result = lookup_table_get(&env->dio_table, (lookup_table_index_t) hal_handle, (void**) &port);

    if (result != LOOKUP_TABLE_SUCCESS) {
        LOGLN("failed to get port from table: %d", result);
        return NULL;
    }

    return port;
}

hal_handle_t insert_port_to_table(hal_env_t* env, dio_port_t* port) {
    hal_handle_t hal_handle = HAL_INVALID_HANDLE;

    lookup_table_result_t result = lookup_table_insert(&(env->dio_table), port,
            (lookup_table_index_t*) &hal_handle);

    if (LOOKUP_TABLE_SUCCESS != result) {
        LOGLN("failed to insert port into table: %d", result);
        return HAL_INVALID_HANDLE;
    }

    return hal_handle;
}

int remove_port_from_table(hal_env_t* env, hal_handle_t hal_handle) {
    lookup_table_result_t result = lookup_table_remove(&env->dio_table, (lookup_table_index_t) hal_handle);

    if (result != LOOKUP_TABLE_SUCCESS) {
        LOGLN("failed to remove port from table: %d", result);
        return 0;
    }

    return 1;
}

int port_id_compare(port_id_t first, port_id_t second) {
    return first == second;
}
