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
    hal_open_port_t open_port;
    uint8_t native_data[];
} hal_open_port_node_t;

struct _hal_env {
    int initialized;
    hal_descriptor_table_t ports_table;
    hal_backend_t backend;
    pthread_mutex_t mutex;
};

int hal_find_port_from_handle(hal_env_t* env, hal_handle_t handle, hal_open_port_node_t** port_out, size_t* index_out);

#ifdef __cplusplus
}
#endif
