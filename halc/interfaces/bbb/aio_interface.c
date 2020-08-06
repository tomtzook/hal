#include <stdio.h>

#include "interface_def.h"
#include "aio_interface.h"


hal_result_t aio_port_init(interface_env_t* env, port_id_t port_id, port_dir_t port_dir) {
    return HAL_UNSUPPORTED_OPERATION;
}
void aio_port_free(interface_env_t* env, port_id_t port_id) {
}

hal_result_t aio_port_write(interface_env_t* env, port_id_t port_id, aio_value_t value) {
    return HAL_UNSUPPORTED_OPERATION;
}
hal_result_t aio_port_read(interface_env_t* env, port_id_t port_id, aio_value_t* value) {
    return HAL_UNSUPPORTED_OPERATION;
}
