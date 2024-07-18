#pragma once

#include "hal.h"


#ifdef __cplusplus
extern "C" {
#endif

hal_error_t halcontrol_register_port(hal_env_t* env, hal_id_t id);
hal_error_t halcontrol_config_port(hal_env_t* env, hal_id_t id, uint32_t supported_types, uint64_t supported_props);
hal_error_t halcontrol_config_port_append(hal_env_t* env, hal_id_t id, uint32_t supported_types, uint64_t supported_props);
hal_error_t halcontrol_config_add_conflicting_port(hal_env_t* env, hal_id_t id, hal_id_t conflicting_id);
hal_error_t halcontrol_config_backend_allocation_size(hal_env_t* env, hal_id_t id, size_t size);
hal_error_t halcontrol_unregister_port(hal_env_t* env, hal_id_t id);

hal_error_t halcontrol_block_port(hal_env_t* env, hal_id_t id, hal_id_t blocker_id);
hal_error_t halcontrol_unblock_port(hal_env_t* env, hal_id_t id);

#ifdef __cplusplus
}
#endif
