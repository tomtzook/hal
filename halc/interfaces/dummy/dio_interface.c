#include <dio_interface.h>
#include <stdio.h>

#include "interface_def.h"


hal_result_t dio_port_init(interface_env_t* env, port_id_t port_id, port_dir_t port_dir) {
    printf("Initialize DIO Port %d (%d) (env %d) \n", port_id, port_dir, env->some_value_in_env);
    return HAL_SUCCESS;
}
void dio_port_free(interface_env_t* env, port_id_t port_id) {
    printf("Free DIO Port %d (env %d) \n", port_id, env->some_value_in_env);
}

hal_result_t dio_port_write(interface_env_t* env, port_id_t port_id, dio_value_t value) {
    printf("Write DIO Port %d (%d) (env %d) \n", port_id, value, env->some_value_in_env);
    return HAL_SUCCESS;
}
hal_result_t dio_port_read(interface_env_t* env, port_id_t port_id, dio_value_t* value) {
    printf("Read DIO Port %d (env %d) \n", port_id, env->some_value_in_env);
    *value = DIO_VALUE_LOW;
    return HAL_SUCCESS;
}
hal_result_t dio_port_pulse(interface_env_t* env, port_id_t port_id, uint64_t length_us) {
    printf("PULSE DIO Port %d for %ld (env %d) \n", port_id, length_us, env->some_value_in_env);
    return HAL_SUCCESS;
}
