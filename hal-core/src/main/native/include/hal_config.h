#pragma once

#include <hal_types.h>


#ifdef __cplusplus
extern "C" {
#endif

hal_error_t hal_port_property_probe(hal_env_t* env, hal_handle_t handle, hal_prop_key_t key, uint32_t* flags);

hal_error_t hal_port_get_property(hal_env_t* env, hal_handle_t handle, hal_prop_key_t key, uint32_t* value);
hal_error_t hal_port_set_property(hal_env_t* env, hal_handle_t handle, hal_prop_key_t key, uint32_t value);

#ifdef __cplusplus
}
#endif
