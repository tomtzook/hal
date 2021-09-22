#include <stdio.h>

#include "hal.h"


void ports_free(hal_env_t* env, struct _ports_native* native) {

}
hal_error_t dio_port_init(hal_env_t* env, dio_port_t* port) {
    return HAL_SUCCESS;
}
void dio_port_free(hal_env_t* env, dio_port_t* port) {

}
hal_error_t dio_port_write(hal_env_t* env, dio_port_t* port, hal_dio_value_t value) {
    return HAL_SUCCESS;
}
hal_error_t dio_port_read(hal_env_t* env, dio_port_t* port, hal_dio_value_t* value) {
    return HAL_SUCCESS;
}

static hal_error_t ports_init(hal_env_t* env, ports_native_t* native) {
    native->free = ports_free;
    native->dio_init = dio_port_init;
    native->dio_free = dio_port_free;
    native->dio_read = dio_port_read;
    native->dio_write = dio_port_write;
    return HAL_SUCCESS;
}

int main() {
    hal_native_t hal_native = {
            .ports_init = ports_init
    };
    hal_env_t* env = NULL;
    hal_error_t status = hal_init(&env, hal_native);
    if (HAL_IS_ERROR(status)) {
        return 1;
    }

    hal_handle_t handle;
    hal_dio_open(env, 1, PORT_DIR_OUTPUT, &handle);
    hal_dio_set(env, handle, HAL_DIO_HIGH);

    hal_quit(&env);

    return 0;
}
