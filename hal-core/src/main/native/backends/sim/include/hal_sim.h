#pragma once

#include <hal.h>
#include <hal_config.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t halsim_port_handle_t;

typedef hal_error_t(*halsim_open_callback_t)(
        hal_env_t* env,
        halsim_port_handle_t port_handle,
        hal_port_type_t type);
typedef hal_error_t(*halsim_close_callback_t)(
        hal_env_t* env,
        halsim_port_handle_t port_handle,
        hal_port_type_t type);
typedef hal_error_t(*halsim_get_prop_callback_t)(
        hal_env_t* env,
        halsim_port_handle_t port_handle,
        hal_prop_key_t key,
        uint32_t* value);
typedef hal_error_t(*halsim_set_prop_callback_t)(
        hal_env_t* env,
        halsim_port_handle_t port_handle,
        hal_prop_key_t key,
        uint32_t value);
typedef hal_error_t(*halsim_dio_get_value_callback_t)(
        hal_env_t* env,
        halsim_port_handle_t port_handle,
        hal_dio_value_t* value);
typedef hal_error_t(*halsim_dio_set_value_callback_t)(
        hal_env_t* env,
        halsim_port_handle_t port_handle,
        hal_dio_value_t value);
typedef hal_error_t(*halsim_aio_get_value_callback_t)(
        hal_env_t* env,
        halsim_port_handle_t port_handle,
        uint32_t* value);
typedef hal_error_t(*halsim_aio_set_value_callback_t)(
        hal_env_t* env,
        halsim_port_handle_t port_handle,
        uint32_t value);
typedef hal_error_t(*halsim_pwm_get_value_callback_t)(
        hal_env_t* env,
        halsim_port_handle_t port_handle,
        uint32_t* value);
typedef hal_error_t(*halsim_pwm_set_value_callback_t)(
        hal_env_t* env,
        halsim_port_handle_t port_handle,
        uint32_t value);
typedef hal_error_t(*halsim_quadrature_get_position_callback_t)(
        hal_env_t* env,
        halsim_port_handle_t port_handle,
        uint32_t* value);
typedef hal_error_t(*halsim_quadrature_set_position_callback_t)(
        hal_env_t* env,
        halsim_port_handle_t port_handle,
        uint32_t value);
typedef hal_error_t(*halsim_quadrature_get_period_callback_t)(
        hal_env_t* env,
        halsim_port_handle_t port_handle,
        uint32_t* value);

hal_error_t halsim_create_port(hal_env_t* env, const char* name, halsim_port_handle_t* port_handle);
hal_error_t halsim_get_handle(hal_env_t* env, const char* name, halsim_port_handle_t* port_handle);

hal_error_t halsim_config_port_types(hal_env_t* env, halsim_port_handle_t port_handle, uint32_t types);
hal_error_t halsim_config_add_conflicting_port(hal_env_t* env, halsim_port_handle_t port_handle, const char* port_name);
hal_error_t halsim_config_port_callbacks(hal_env_t* env, halsim_port_handle_t port_handle,
                                         halsim_open_callback_t open_callback,
                                         halsim_close_callback_t close_callback);
hal_error_t halsim_config_port_prop(hal_env_t* env, halsim_port_handle_t port_handle, hal_prop_key_t key, uint32_t flags);
hal_error_t halsim_config_port_prop_callbacks(hal_env_t* env, halsim_port_handle_t port_handle, hal_prop_key_t key,
                                              halsim_get_prop_callback_t get_callback,
                                              halsim_set_prop_callback_t set_callback);

hal_error_t halsim_port_get_prop(hal_env_t* env, halsim_port_handle_t port_handle, hal_prop_key_t key, uint32_t* value);
hal_error_t halsim_port_set_prop(hal_env_t* env, halsim_port_handle_t port_handle, hal_prop_key_t key, uint32_t value);

hal_error_t halsim_dio_config_callbacks(hal_env_t* env, halsim_port_handle_t port_handle,
                                        halsim_dio_get_value_callback_t get_value_callback,
                                        halsim_dio_set_value_callback_t set_value_callback);
hal_error_t halsim_dio_get_value(hal_env_t* env, halsim_port_handle_t port_handle, hal_dio_value_t* value);
hal_error_t halsim_dio_set_value(hal_env_t* env, halsim_port_handle_t port_handle, hal_dio_value_t value);

hal_error_t halsim_aio_config_callbacks(hal_env_t* env, halsim_port_handle_t port_handle,
                                        halsim_aio_get_value_callback_t get_value_callback,
                                        halsim_aio_set_value_callback_t set_value_callback);
hal_error_t halsim_aio_get_value(hal_env_t* env, halsim_port_handle_t port_handle, uint32_t* value);
hal_error_t halsim_aio_set_value(hal_env_t* env, halsim_port_handle_t port_handle, uint32_t value);

hal_error_t halsim_pwm_config_callbacks(hal_env_t* env, halsim_port_handle_t port_handle,
                                        halsim_pwm_get_value_callback_t get_value_callback,
                                        halsim_pwm_set_value_callback_t set_value_callback);
hal_error_t halsim_pwm_get_value(hal_env_t* env, halsim_port_handle_t port_handle, uint32_t* value);
hal_error_t halsim_pwm_set_value(hal_env_t* env, halsim_port_handle_t port_handle, uint32_t value);

hal_error_t halsim_quadrature_config_callbacks(hal_env_t* env, halsim_port_handle_t port_handle,
                                               halsim_quadrature_get_position_callback_t get_position_callback,
                                               halsim_quadrature_set_position_callback_t set_position_callback,
                                               halsim_quadrature_get_period_callback_t get_period_callback);
hal_error_t halsim_quadrature_get_position(hal_env_t* env, halsim_port_handle_t port_handle, uint32_t* value);
hal_error_t halsim_quadrature_set_position(hal_env_t* env, halsim_port_handle_t port_handle, uint32_t value);
hal_error_t halsim_quadrature_get_period(hal_env_t* env, halsim_port_handle_t port_handle, uint32_t* value);


#ifdef __cplusplus
}
#endif
