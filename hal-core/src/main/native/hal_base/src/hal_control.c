
#include <malloc.h>

#include <hal.h>

#include "hal_control.h"
#include "hal_internal.h"


hal_error_t halcontrol_register_port(hal_env_t* env, hal_id_t id) {
    HAL_CHECK_INITIALIZED(env);

    if (id == HAL_INVALID_IDENTIFIER || id > env->port_table.capacity) {
        HAL_RETURN_IF_ERROR(HAL_ERROR_BAD_ARGUMENT);
    }

    hal_error_t status = HAL_SUCCESS;

    size_t index = (size_t) id;
    hal_port_t* port;
    if (!hal_descriptor_table_get(&env->port_table, index, (void**) &port)) {
        HAL_RETURN_IF_ERROR(HAL_ERROR_TAKEN);
    }

    port = (hal_port_t*) malloc(sizeof(hal_port_t));
    HAL_CHECK_ALLOCATED(port, end);

    if (hal_descriptor_table_insert(&env->port_table, port, index)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_DATA, end);
    }

    TRACE_INFO("Registering port %u", id);

    port->identifier = id;
    port->supported_types = 0;
    port->supported_props = 0;
    port->flags = 0;
    port->open_handle = HAL_EMPTY_HANDLE;
    port->blocker_id = HAL_INVALID_IDENTIFIER;

end:
    if (HAL_IS_ERROR(status)) {
        free(port);
    }

    return status;
}

hal_error_t halcontrol_config_port(hal_env_t* env, hal_id_t id, uint32_t supported_types, uint64_t supported_props) {
    HAL_CHECK_INITIALIZED(env);

    size_t index = (size_t) id;
    hal_port_t* port;
    if (hal_descriptor_table_get(&env->port_table, index, (void**) &port)) {
        HAL_RETURN_IF_ERROR(HAL_ERROR_NOT_FOUND);
    }

    if ((port->flags & HAL_FLAG_OPEN) != 0) {
        TRACE_ERROR("Cannot config port %u because it is open", id);
        HAL_RETURN_IF_ERROR(HAL_ERROR_UNSUPPORTED_OPERATION);
    }

    TRACE_INFO("Configuring port %u", id);

    if (supported_types != HAL_OPT_UINT32) {
        port->supported_types = supported_types;
    }

    if (supported_props != HAL_OPT_UINT64) {
        port->supported_props = supported_props;
    }

    return HAL_SUCCESS;
}

hal_error_t halcontrol_config_port_append(hal_env_t* env, hal_id_t id, uint32_t supported_types, uint64_t supported_props) {
    HAL_CHECK_INITIALIZED(env);

    size_t index = (size_t) id;
    hal_port_t* port;
    if (hal_descriptor_table_get(&env->port_table, index, (void**) &port)) {
        HAL_RETURN_IF_ERROR(HAL_ERROR_NOT_FOUND);
    }

    if ((port->flags & HAL_FLAG_OPEN) != 0) {
        TRACE_ERROR("Cannot config port %u because it is open", id);
        HAL_RETURN_IF_ERROR(HAL_ERROR_UNSUPPORTED_OPERATION);
    }

    TRACE_INFO("Configuring port %u", id);

    if (supported_types != HAL_OPT_UINT32) {
        port->supported_types |= supported_types;
    }

    if (supported_props != HAL_OPT_UINT64) {
        port->supported_props |= supported_props;
    }

    return HAL_SUCCESS;
}

hal_error_t halcontrol_unregister_port(hal_env_t* env, hal_id_t id) {
    HAL_CHECK_INITIALIZED(env);

    TRACE_INFO("Unregister port %u", id);

    size_t index = (size_t) id;
    hal_port_t* port;
    if (hal_descriptor_table_get(&env->port_table, index, (void**) &port)) {
        HAL_RETURN_IF_ERROR(HAL_ERROR_NOT_FOUND);
    }

    if ((port->flags & HAL_FLAG_OPEN) != 0) {
        TRACE_ERROR("Cannot unregister port %u because it is open", id);
        HAL_RETURN_IF_ERROR(HAL_ERROR_UNSUPPORTED_OPERATION);
    }

    hal_descriptor_table_remove(&env->port_table, index);
    free(port);

    return HAL_SUCCESS;
}

hal_error_t halcontrol_block_port(hal_env_t* env, hal_id_t id, hal_id_t blocker_id) {
    HAL_CHECK_INITIALIZED(env);

    TRACE_INFO("Blocking port %u for port %u", id, blocker_id);

    hal_port_t* port;
    if (hal_find_port_by_id(env, id, &port)) {
        HAL_RETURN_IF_ERROR(HAL_ERROR_NOT_FOUND);
    }

    hal_port_t* port_blocker;
    if (hal_find_port_by_id(env, blocker_id, &port_blocker)) {
        HAL_RETURN_IF_ERROR(HAL_ERROR_NOT_FOUND);
    }

    if ((port->flags & HAL_FLAG_OPEN) != 0) {
        TRACE_ERROR("Cannot block open port %u", id);
        HAL_RETURN_IF_ERROR(HAL_ERROR_OPERATION_BAD_STATE);
    }

    port->flags |= HAL_FLAG_BLOCKED;
    port->blocker_id = blocker_id;

    return HAL_SUCCESS;
}

hal_error_t halcontrol_unblock_port(hal_env_t* env, hal_id_t id) {
    HAL_CHECK_INITIALIZED(env);

    TRACE_INFO("Unblocking port %u", id);

    hal_port_t* port;
    if (hal_find_port_by_id(env, id, &port)) {
        HAL_RETURN_IF_ERROR(HAL_ERROR_NOT_FOUND);
    }

    if ((port->flags & HAL_FLAG_BLOCKED) == 0) {
        TRACE_ERROR("Cannot unblock port %u which is not blocked", id);
        HAL_RETURN_IF_ERROR(HAL_ERROR_OPERATION_BAD_STATE);
    }

    port->flags &= ~HAL_FLAG_BLOCKED;
    port->blocker_id = HAL_INVALID_IDENTIFIER;

    return HAL_SUCCESS;
}
