#pragma once

#include <pthread.h>

#include <hal.h>

#include "hal_list.h"
#include "hal_table.h"
#include "hal_error_handling.h"
#include "hal_backend.h"


#ifdef __cplusplus
extern "C" {
#endif

#define HAL_DESCRIPTOR_TABLE_SIZE 256

typedef struct {
    hal_id_t identifier;
    uint32_t supported_types;
    uint64_t supported_props;
    uint32_t flags;

    hal_handle_t open_handle;
    hal_id_t blocker_id;
} hal_port_t;

typedef struct {
    hal_handle_t handle;
    hal_port_t* port;
    hal_backend_port_t backend_port;

    uint8_t native_data[];
} hal_open_port_node_t;

struct hal__env {
    int initialized;
    hal_descriptor_table_t port_table;
    hal_descriptor_table_t handle_table;
    hal_backend_t backend;
    pthread_mutex_t mutex;
};

int hal_find_port_by_id(hal_env_t* env, hal_id_t id, hal_port_t** port_out);
int hal_find_next_id(hal_env_t* env, hal_id_t* id);

int hal_find_port_from_handle(hal_env_t* env, hal_handle_t handle, hal_open_port_node_t** port_out, size_t* index_out);

#ifdef __cplusplus
}
#endif
