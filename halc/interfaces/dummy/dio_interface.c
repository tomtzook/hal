#include <dio_interface.h>
#include <stdio.h>

#include "interface_def.h"


int dio_port_init(interface_env_t* env, port_id_t port_id, port_dir_t port_dir) {
    printf("Initialize DIO Port %d (%d) (env %d) \n", port_id, port_dir, env->some_value_in_env);
    return 0;
}
void dio_port_free(interface_env_t* env, port_id_t port_id) {
    printf("Free DIO Port %d (env %d) \n", port_id, env->some_value_in_env);
}

void dio_port_write(interface_env_t* env, port_id_t port_id, dio_value_t value) {
    printf("Write DIO Port %d (%d) (env %d) \n", port_id, value, env->some_value_in_env);
}
dio_value_t dio_port_read(interface_env_t* env, port_id_t port_id) {
    printf("Read DIO Port %d (env %d) \n", port_id, env->some_value_in_env);
    return LOW;
}
