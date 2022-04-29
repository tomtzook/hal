
#include <hal.h>

static hal_error_t ports_init(hal_env_t* env, void** data) {
    return HAL_SUCCESS;
}

static void ports_free(hal_env_t* env) {

}

static hal_error_t ports_probe(hal_env_t* env, hal_port_t port, uint8_t flags) {
    return HAL_SUCCESS;
}

static hal_error_t dio_port_init(hal_env_t* env, dio_port_t* port) {
    return HAL_ERROR_UNSUPPORTED_OPERATION;
}

static void dio_port_free(hal_env_t* env, dio_port_t* port) {

}

static hal_error_t dio_port_write(hal_env_t* env, dio_port_t* port, hal_dio_value_t value) {
    return HAL_ERROR_UNSUPPORTED_OPERATION;
}

static hal_error_t dio_port_read(hal_env_t* env, dio_port_t* port, hal_dio_value_t* value) {
    return HAL_ERROR_UNSUPPORTED_OPERATION;
}

static hal_error_t aio_port_init(hal_env_t* env, aio_port_t* port) {
    return HAL_ERROR_UNSUPPORTED_OPERATION;
}

static void aio_port_free(hal_env_t* env, aio_port_t* port) {

}

static hal_error_t aio_port_write(hal_env_t* env, aio_port_t* port, hal_aio_value_t value) {
    return HAL_ERROR_UNSUPPORTED_OPERATION;
}

static hal_error_t aio_port_read(hal_env_t* env, aio_port_t* port, hal_aio_value_t* value) {
    return HAL_ERROR_UNSUPPORTED_OPERATION;
}

hal_aio_value_t aio_max_value(hal_env_t* env) {
    return 0;
}

ports_native_interface _ports_native_interface = {
    .init = ports_init,
    .free = ports_free,
    .probe = ports_probe,
    .dio_init = dio_port_init,
    .dio_free = dio_port_free,
    .dio_write = dio_port_write,
    .dio_read = dio_port_read,
    .aio_init = aio_port_init,
    .aio_free = aio_port_free,
    .aio_write = aio_port_write,
    .aio_read = aio_port_read,
    .aio_max_value = aio_max_value
};