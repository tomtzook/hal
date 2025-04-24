
#include <malloc.h>
#include <string.h>

#include <hal.h>
#include "hal_error_handling.h"
#include "hal_internal.h"
#include "hal_control.h"


hal_error_t halcontrol_register_port(hal_env_t* env, const hal_id_t id) {
    HAL_CHECK_SERVICES_INITIALIZED(env);

    if (id == HAL_INVALID_IDENTIFIER || id > env->port_table.capacity) {
        HAL_RETURN_IF_ERROR(HAL_ERROR_BAD_ARGUMENT);
    }

    hal_error_t status = HAL_SUCCESS;

    pthread_mutex_lock(&env->mutex);

    const size_t index = (size_t) id;
    hal_port_t* port;
    if (!hal_descriptor_table_get(&env->port_table, index, (void**) &port)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_TAKEN, end);
    }

    port = (hal_port_t*) malloc(sizeof(hal_port_t));
    HAL_CHECK_ALLOCATED(port, end);

    if (hal_descriptor_table_insert(&env->port_table, port, index)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_DATA, end);
    }

    TRACE_INFO("Registering port 0x%x", id);

    port->identifier = id;
    port->supported_types = 0;
    port->supported_props = 0;
    port->flags = 0;
    port->open_handle = HAL_EMPTY_HANDLE;
    port->blocker_id = HAL_INVALID_IDENTIFIER;
    port->backend_extra_allocation_size = 0;
    port->conflicting.next_index = 0;

    memset(port->name, 0, sizeof(port->name));

end:
    if (HAL_IS_ERROR(status)) {
        free(port);
    }

    pthread_mutex_unlock(&env->mutex);

    return status;
}

hal_error_t halcontrol_config_port_name(hal_env_t* env, const hal_id_t id, const char* name) {
    HAL_CHECK_SERVICES_INITIALIZED(env);

    hal_error_t status = HAL_SUCCESS;

    pthread_mutex_lock(&env->mutex);

    const size_t index = (size_t) id;
    hal_port_t* port;
    if (hal_descriptor_table_get(&env->port_table, index, (void**) &port)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    TRACE_INFO("Configuring port 0x%x with name %s", id, name);

    const size_t name_length = strlen(name);
    if (name_length >= sizeof(port->name) - 1) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NO_SPACE, end);
    }
    if (name[0] == '\0') {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_ARGUMENT, end);
    }

    memcpy(port->name, name, name_length);
    port->name[name_length] = '\0';

end:
    pthread_mutex_unlock(&env->mutex);
    return status;
}

hal_error_t halcontrol_config_port(hal_env_t* env, const hal_id_t id, const uint32_t supported_types, const uint64_t supported_props) {
    HAL_CHECK_SERVICES_INITIALIZED(env);

    hal_error_t status = HAL_SUCCESS;

    pthread_mutex_lock(&env->mutex);

    const size_t index = (size_t) id;
    hal_port_t* port;
    if (hal_descriptor_table_get(&env->port_table, index, (void**) &port)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    if ((port->flags & HAL_FLAG_OPEN) != 0) {
        TRACE_ERROR("Cannot config port 0x%x because it is open", id);
        HAL_JUMP_IF_ERROR(HAL_ERROR_UNSUPPORTED_OPERATION, end);
    }

    TRACE_INFO("Configuring port 0x%x", id);

    if (supported_types != HAL_OPT_UINT32) {
        port->supported_types = supported_types;
    }

    if (supported_props != HAL_OPT_UINT64) {
        port->supported_props = supported_props;
    }

end:
    pthread_mutex_unlock(&env->mutex);
    return status;
}

hal_error_t halcontrol_config_port_append(hal_env_t* env, const hal_id_t id, const uint32_t supported_types, const uint64_t supported_props) {
    HAL_CHECK_SERVICES_INITIALIZED(env);

    hal_error_t status = HAL_SUCCESS;

    pthread_mutex_lock(&env->mutex);

    const size_t index = (size_t) id;
    hal_port_t* port;
    if (hal_descriptor_table_get(&env->port_table, index, (void**) &port)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    if ((port->flags & HAL_FLAG_OPEN) != 0) {
        TRACE_ERROR("Cannot config port 0x%x because it is open", id);
        HAL_JUMP_IF_ERROR(HAL_ERROR_UNSUPPORTED_OPERATION, end);
    }

    TRACE_INFO("Configuring port 0x%x", id);

    if (supported_types != HAL_OPT_UINT32) {
        port->supported_types |= supported_types;
    }

    if (supported_props != HAL_OPT_UINT64) {
        port->supported_props |= supported_props;
    }

end:
    pthread_mutex_unlock(&env->mutex);
    return status;
}

hal_error_t halcontrol_config_add_conflicting_port(hal_env_t* env, const hal_id_t id, const hal_id_t conflicting_id) {
    HAL_CHECK_SERVICES_INITIALIZED(env);

    hal_error_t status = HAL_SUCCESS;

    pthread_mutex_lock(&env->mutex);

    const size_t index = (size_t) id;
    hal_port_t* port;
    if (hal_descriptor_table_get(&env->port_table, index, (void**) &port)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    if ((port->flags & HAL_FLAG_OPEN) != 0) {
        TRACE_ERROR("Cannot config port 0x%x because it is open", id);
        HAL_JUMP_IF_ERROR(HAL_ERROR_UNSUPPORTED_OPERATION, end);
    }

    TRACE_INFO("Configuring on port 0x%x conflicting 0x%x", id, conflicting_id);

    if (port->conflicting.next_index >= HAL_MAX_CONFLICTING) {
        HAL_RETURN_IF_ERROR(HAL_ERROR_NO_SPACE);
    }

    port->conflicting.list[port->conflicting.next_index] = conflicting_id;
    port->conflicting.next_index++;

end:
    pthread_mutex_unlock(&env->mutex);
    return status;
}

hal_error_t halcontrol_config_backend_allocation_size(hal_env_t* env, const hal_id_t id, const size_t size) {
    HAL_CHECK_SERVICES_INITIALIZED(env);

    hal_error_t status = HAL_SUCCESS;

    pthread_mutex_lock(&env->mutex);

    const size_t index = (size_t) id;
    hal_port_t* port;
    if (hal_descriptor_table_get(&env->port_table, index, (void**) &port)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    if ((port->flags & HAL_FLAG_OPEN) != 0) {
        TRACE_ERROR("Cannot config port 0x%x because it is open", id);
        HAL_JUMP_IF_ERROR(HAL_ERROR_UNSUPPORTED_OPERATION, end);
    }

    TRACE_INFO("Configuring on port 0x%x with backend allocation size 0x%lx", id, size);

    port->backend_extra_allocation_size = size;

end:
    pthread_mutex_unlock(&env->mutex);
    return status;
}

hal_error_t halcontrol_unregister_port(hal_env_t* env, const hal_id_t id) {
    HAL_CHECK_SERVICES_INITIALIZED(env);

    hal_error_t status = HAL_SUCCESS;

    pthread_mutex_lock(&env->mutex);

    TRACE_INFO("Unregister port 0x%x", id);

    const size_t index = (size_t) id;
    hal_port_t* port;
    if (hal_descriptor_table_get(&env->port_table, index, (void**) &port)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    if ((port->flags & HAL_FLAG_OPEN) != 0) {
        TRACE_ERROR("Cannot unregister port 0x%x because it is open", id);
        HAL_JUMP_IF_ERROR(HAL_ERROR_UNSUPPORTED_OPERATION, end);
    }

    hal_descriptor_table_remove(&env->port_table, index);
    free(port);

end:
    pthread_mutex_unlock(&env->mutex);
    return status;
}

hal_error_t halcontrol_block_port(hal_env_t* env, const hal_id_t id, const hal_id_t blocker_id) {
    HAL_CHECK_SERVICES_INITIALIZED(env);

    hal_error_t status = HAL_SUCCESS;

    pthread_mutex_lock(&env->mutex);

    TRACE_INFO("Blocking port 0x%x for port 0x%x", id, blocker_id);

    hal_port_t* port;
    if (hal_find_port_by_id(env, id, &port)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    hal_port_t* port_blocker;
    if (hal_find_port_by_id(env, blocker_id, &port_blocker)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    if ((port->flags & HAL_FLAG_OPEN) != 0) {
        TRACE_ERROR("Cannot block open port 0x%x", id);
        HAL_JUMP_IF_ERROR(HAL_ERROR_OPERATION_BAD_STATE, end);
    }

    if ((port->flags & HAL_FLAG_BLOCKED) != 0) {
        if (port->blocker_id == blocker_id) {
            // already blocked for requested
            goto end;
        } else {
            TRACE_ERROR("port 0x%x already blocked by 0x%x", id, port->blocker_id);
            HAL_JUMP_IF_ERROR(HAL_ERROR_OPERATION_BAD_STATE, end);
        }
    }

    port->flags |= HAL_FLAG_BLOCKED;
    port->blocker_id = blocker_id;

end:
    pthread_mutex_unlock(&env->mutex);
    return status;
}

hal_error_t halcontrol_unblock_port(hal_env_t* env, const hal_id_t id) {
    HAL_CHECK_SERVICES_INITIALIZED(env);

    hal_error_t status = HAL_SUCCESS;

    pthread_mutex_lock(&env->mutex);

    TRACE_INFO("Unblocking port 0x%x", id);

    hal_port_t* port;
    if (hal_find_port_by_id(env, id, &port)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    if ((port->flags & HAL_FLAG_BLOCKED) == 0) {
        TRACE_ERROR("Cannot unblock port 0x%x which is not blocked", id);
        HAL_JUMP_IF_ERROR(HAL_ERROR_OPERATION_BAD_STATE, end);
    }

    port->flags &= ~HAL_FLAG_BLOCKED;
    port->blocker_id = HAL_INVALID_IDENTIFIER;

end:
    pthread_mutex_unlock(&env->mutex);
    return status;
}
