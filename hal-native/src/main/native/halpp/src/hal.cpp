
#include <hal.h>

#include "halpp.h"


namespace hal {

generic_port::~generic_port() {
    hal_close(m_env, m_handle);
}

bool port<HAL_TYPE_DIGITAL_INPUT>::get() {
    hal_dio_value_t value;
    HAL_CALL(hal_dio_get(m_env, m_handle, &value));
    return value == HAL_DIO_HIGH;
}

bool port<HAL_TYPE_DIGITAL_OUTPUT>::get() {
    hal_dio_value_t value;
    HAL_CALL(hal_dio_get(m_env, m_handle, &value));
    return value == HAL_DIO_HIGH;
}

void port<HAL_TYPE_DIGITAL_OUTPUT>::set(bool value) {
    hal_dio_value_t dio_value = value ? HAL_DIO_HIGH : HAL_DIO_LOW;
    HAL_CALL(hal_dio_set(m_env, m_handle, dio_value));
}

env::~env() {
    hal_shutdown(m_env);
}

hal_handle_t env::open(hal_port_t port, hal_port_type_t type) {
    hal_handle_t handle;
    HAL_CALL(hal_open(m_env, port, type, &handle));

    return handle;
}

env init() {
    hal_env_t* _env;
    HAL_CALL(hal_init(&_env));

    return env{_env};
}

}
