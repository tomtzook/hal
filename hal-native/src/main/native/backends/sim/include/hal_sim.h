#pragma once

#include <hal.h>
#include <hal_config.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t halsim_port_handle_t;

typedef hal_error_t(*halsim_get_prop_callback_t)(
        hal_env_t* env,
        halsim_port_handle_t port_handle,
        hal_prop_key_t key,
        hal_prop_value_t* value);
typedef hal_error_t(*halsim_set_prop_callback_t)(
        hal_env_t* env,
        halsim_port_handle_t port_handle,
        hal_prop_key_t key,
        hal_prop_value_t value);

hal_error_t halsim_create_port(hal_env_t* env, const char* name, halsim_port_handle_t* port_handle);

hal_error_t halsim_config_port_types(hal_env_t* env, halsim_port_handle_t port_handle, uint32_t types);
hal_error_t halsim_config_port_prop(hal_env_t* env, halsim_port_handle_t port_handle, hal_prop_key_t key, uint32_t flags);
hal_error_t halsim_config_port_prop_callbacks(hal_env_t* env, halsim_port_handle_t port_handle, hal_prop_key_t key,
                                              halsim_get_prop_callback_t get_callback,
                                              halsim_set_prop_callback_t set_callback);

hal_error_t halsim_port_get_prop(hal_env_t* env, halsim_port_handle_t port_handle, hal_prop_key_t key, hal_prop_value_t* value);
hal_error_t halsim_port_set_prop(hal_env_t* env, halsim_port_handle_t port_handle, hal_prop_key_t key, hal_prop_value_t value);

// need specific port handles?
//hal_error_t halsim_port_get_prop()
//hal_error_t halsim_port_set_prop(hal_env_t* env, halsim_port_handle_t handle, hal_prop_key_t key, hal_prop_value_t value);

#ifdef __cplusplus
}
#endif
