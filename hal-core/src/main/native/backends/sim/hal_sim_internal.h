#pragma once

#include <pthread.h>

#include <hal.h>
#include <hal_config.h>
#include <hal_table.h>

#include "include/hal_sim.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_PORTS 256

typedef struct {
    int configured;
    hal_prop_key_t key;
    uint32_t flags;
    halsim_get_prop_callback_t get_callback;
    halsim_set_prop_callback_t set_callback;
} halsim_port_prop_config_t;

typedef struct {
    char name[HAL_PORT_NAME_MAX];
    uint32_t supported_types;
    halsim_port_handle_t handle;
    halsim_port_prop_config_t props_config[HAL_CONFIG_KEY_MAX];
    halsim_open_callback_t open_callback;
    halsim_close_callback_t close_callback;

    int is_open;
    hal_port_type_t open_type;
    uint32_t props_values[HAL_CONFIG_KEY_MAX];

    union {
        hal_dio_value_t dio_value;
        uint32_t aio_value;
        uint32_t pwm_value;
    } value;

    struct {
        halsim_dio_get_value_callback_t get_value;
        halsim_dio_set_value_callback_t set_value;
    } dio_callbacks;

    struct {
        halsim_aio_get_value_callback_t get_value;
        halsim_aio_set_value_callback_t set_value;
    } aio_callbacks;

    struct {
        halsim_pwm_get_value_callback_t get_value;
        halsim_pwm_set_value_callback_t set_value;
    } pwm_callbacks;
} halsim_port_t;

typedef struct {
    hal_descriptor_table_t ports;
    pthread_mutex_t mutex;
    pthread_mutexattr_t mutex_attr;
} halsim_data_t;

halsim_data_t* get_global_data(hal_backend_t* env);
halsim_data_t* get_global_data_from_env(hal_env_t* env);

int find_sim_port_index(hal_backend_t* env, const char* port_name, size_t* index);
int find_next_sim_port_from_index(hal_backend_t* env, size_t start_index, halsim_port_t** port_out, size_t* index);
int find_sim_port_from_handle(hal_backend_t* env, halsim_port_handle_t handle, halsim_port_t** port_out, size_t* index_out);

#ifdef __cplusplus
}
#endif
