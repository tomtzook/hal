
#include <hal.h>

#include "halpp_ports.h"


namespace hal {

generic_port::~generic_port() {
    if (nullptr != m_env && HAL_EMPTY_HANDLE != m_handle) {
        hal_close(m_env, m_handle);
    }
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

}

