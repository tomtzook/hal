#include <stdlib.h>

#include "hal_aio.h"
#include "hal_global.h"
#include "logging/log.h"
#include "interface/aio_interface.h"

#include "hal_modules.h"
#include "port_table.h"

typedef struct {
    port_id_t port_id;
    port_dir_t port_dir;

    aio_value_t last_value;
} aio_port_t;

hal_result_t hal_aio_init_module(hal_env_t* env) {
    lookup_table_result_t table_init_result = lookup_table_init(&env->aio_table);
    if (LOOKUP_TABLE_SUCCESS != table_init_result) {
        LOGLN("failed to initialize lookup table: %d", table_init_result);
        return HAL_INITIALIZATION_ERROR;
    }

    return HAL_SUCCESS;
}

void hal_aio_free_module(hal_env_t* env) {
    if (HAL_NOT_INITIALIZED(env)) {
        return;
    }

    lookup_table_free(&env->aio_table);
}

hal_result_t hal_aio_init(hal_env_t* env, port_id_t port_id, port_dir_t port_dir, hal_handle_t* result_handle) {
    if (HAL_NOT_INITIALIZED(env)) {
        return HAL_NOT_INITIALIZED;
    }

    hal_result_t result;
    aio_port_t* port;
    hal_handle_t port_handle;

    result = table_find_port(&env->aio_table, port_id, &port_handle);
    if (HAL_SUCCESS == result) {
        result = table_get_port(&env->aio_table, port_handle, (void**) &port);
        if (HAL_SUCCESS != result) {
            return result;
        }
        if (port_dir != port->port_dir) {
            return HAL_ARGUMENT_ERROR;
        }
    }
    else if (HAL_PORT_NOT_FOUND == result) {
        port = malloc(sizeof(aio_port_t));
        if (NULL == port) {
            return HAL_MEMORY_ALLOCATION_ERROR;
        }

        port->port_dir = port_dir;
        port->port_id = port_id;
        port->last_value = 0;

        result = table_insert_port(&env->aio_table, port, &port_handle);
        if (HAL_SUCCESS != result) {
            free(port);

            return result;
        }

        result = aio_port_init(env->interface_env, port_id, port_dir);
        if (HAL_SUCCESS != result) {
            table_remove_port(&env->aio_table, port_handle);
            free(port);

            return result;
        }
    } else {
        return result;
    }

    *result_handle = port_handle;
    return HAL_SUCCESS;
}
hal_result_t hal_aio_free(hal_env_t* env, hal_handle_t port_handle) {
    if (HAL_NOT_INITIALIZED(env)) {
        return HAL_NOT_INITIALIZED;
    }

    hal_result_t result;
    aio_port_t* port;

    result = table_get_port(&env->aio_table, port_handle, (void**) &port);
    if (HAL_SUCCESS != result) {
        return result;
    }

    result = table_remove_port(&env->aio_table, port_handle);
    if (HAL_SUCCESS != result) {
        return HAL_STORE_ERROR;
    }

    port_id_t port_id = port->port_id;

    free(port);
    aio_port_free(env->interface_env, port_id);

    return HAL_SUCCESS;
}

hal_result_t hal_aio_set(hal_env_t* env, hal_handle_t port_handle, aio_value_t value) {
    if (HAL_NOT_INITIALIZED(env)) {
        return HAL_NOT_INITIALIZED;
    }

    hal_result_t result;
    aio_port_t* port;

    result = table_get_port(&env->aio_table, port_handle, (void**) &port);
    if (HAL_SUCCESS != result) {
        return result;
    }

    if (PORT_DIR_OUTPUT != port->port_dir) {
        return HAL_ARGUMENT_ERROR;
    }

    result = aio_port_write(env->interface_env, port->port_id, value);
    if (HAL_SUCCESS != result) {
        return result;
    }

    port->last_value = value;

    return HAL_SUCCESS;
}
hal_result_t hal_aio_get(hal_env_t* env, hal_handle_t port_handle, aio_value_t* result_value) {
    if (HAL_NOT_INITIALIZED(env)) {
        return HAL_NOT_INITIALIZED;
    }

    hal_result_t result;
    aio_port_t* port;

    result = table_get_port(&env->aio_table, port_handle, (void**) &port);
    if (HAL_SUCCESS != result) {
        return result;
    }

    if (port->port_dir == PORT_DIR_OUTPUT) {
        *result_value = port->last_value;
    } else {
        result = aio_port_read(env->interface_env, port->port_id, result_value);
        if (result) {
            return result;
        }
    }

    return HAL_SUCCESS;
}