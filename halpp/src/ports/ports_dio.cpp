#include "halpp_except.hpp"

#include "ports/ports_dio.hpp"

namespace hal {

digital_output::digital_output(hal_env_t* env, hal_port_t port)
    : m_env(env)
    , m_handle(HAL_EMPTY_HANDLE) {
    HAL_CHECK(hal_dio_open(m_env, port, PORT_DIR_OUTPUT, &m_handle));
}

digital_output::digital_output(digital_output&& other) noexcept
    : digital_output(other.m_env, other.m_handle) {
    other.m_handle = HAL_EMPTY_HANDLE;
}

digital_output::digital_output(hal_env_t* env, hal_handle_t handle)
    : m_env(env)
    , m_handle(handle) {
}

digital_output::~digital_output() {
    if (HAL_EMPTY_HANDLE != m_handle) {
        hal_dio_close(m_env, m_handle);
    }
}

digital_output& digital_output::operator=(digital_output&& other) noexcept {
    m_env = other.m_env;
    m_handle = other.m_handle;
    other.m_handle = HAL_EMPTY_HANDLE;

    return *this;
}

void digital_output::set(hal_dio_value_t value) {
    HAL_CHECK(hal_dio_set(m_env, m_handle, value));
}

hal_dio_value_t digital_output::get() {
    hal_dio_value_t value;
    HAL_CHECK(hal_dio_get(m_env, m_handle, &value));

    return value;
}

digital_input::digital_input(hal_env_t* env, hal_port_t port)
        : m_env(env)
        , m_handle(HAL_EMPTY_HANDLE) {
    HAL_CHECK(hal_dio_open(m_env, port, PORT_DIR_INPUT, &m_handle));
}

digital_input::digital_input(digital_input&& other) noexcept
        : digital_input(other.m_env, other.m_handle) {
    other.m_handle = HAL_EMPTY_HANDLE;
}

digital_input::digital_input(hal_env_t* env, hal_handle_t handle)
        : m_env(env)
        , m_handle(handle) {
}

digital_input::~digital_input() {
    if (HAL_EMPTY_HANDLE != m_handle) {
        hal_dio_close(m_env, m_handle);
    }
}

digital_input& digital_input::operator=(digital_input&& other) noexcept {
    m_env = other.m_env;
    m_handle = other.m_handle;
    other.m_handle = HAL_EMPTY_HANDLE;

    return *this;
}

hal_dio_value_t digital_input::get() {
    hal_dio_value_t value;
    HAL_CHECK(hal_dio_get(m_env, m_handle, &value));

    return value;
}

}
