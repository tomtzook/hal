#include <stdio.h>

#include "interface_def.h"
#include "aio_interface.h"

hal_result_t aio_maxvalue(interface_env_t* env, aio_value_t* value) {
    *value = 4095;
    return HAL_SUCCESS;
}
hal_result_t aio_minvalue(interface_env_t* env, aio_value_t* value) {
    *value = 0;
    return HAL_SUCCESS;
}

hal_result_t aio_port_init(interface_env_t* env, port_id_t port_id, port_dir_t port_dir) {
    printf("Initialize AIO Port %d (%d) (env %d) \n", port_id, port_dir, env->some_value_in_env);
    return HAL_SUCCESS;
}
void aio_port_free(interface_env_t* env, port_id_t port_id) {
    printf("Free AIO Port %d (env %d) \n", port_id, env->some_value_in_env);
}

hal_result_t aio_port_write(interface_env_t* env, port_id_t port_id, aio_value_t value) {
    printf("Write AIO Port %d (%ld) (env %d) \n", port_id, value, env->some_value_in_env);
    return HAL_SUCCESS;
}
hal_result_t aio_port_read(interface_env_t* env, port_id_t port_id, aio_value_t* value) {
    *value = 0;

    printf("Read AIO Port %d (env %d) \n", port_id, env->some_value_in_env);
    return HAL_SUCCESS;
}
