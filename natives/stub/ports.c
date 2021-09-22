
#include <hal.h>
#include "hal_stub.h"


static void ports_free(hal_env_t* env, struct _ports_native* native) {

}

static hal_error_t dio_port_init(hal_env_t* env, dio_port_t* port) {
    return HAL_UNSUPPORTED_OPERATION;
}

static void dio_port_free(hal_env_t* env, dio_port_t* port) {

}

static hal_error_t dio_port_write(hal_env_t* env, dio_port_t* port, hal_dio_value_t value) {
    return HAL_UNSUPPORTED_OPERATION;
}

static hal_error_t dio_port_read(hal_env_t* env, dio_port_t* port, hal_dio_value_t* value) {
    return HAL_UNSUPPORTED_OPERATION;
}

hal_error_t ports_init(hal_env_t* env, ports_native_t* native) {
    native->free = ports_free;
    native->dio_init = dio_port_init;
    native->dio_free = dio_port_free;
    native->dio_read = dio_port_read;
    native->dio_write = dio_port_write;

    return HAL_SUCCESS;
}
