#pragma once

#include <ports/ports_dio.h>

namespace hal {

class digital_output {
public:
    digital_output(hal_env_t* m_env, hal_port_t port);
    digital_output(digital_output& other) = delete;
    digital_output(digital_output&& other) noexcept;
    digital_output(hal_env_t* env, hal_handle_t handle);
    ~digital_output();

    digital_output& operator=(digital_output& other) = delete;
    digital_output& operator=(digital_output&& other) noexcept;

    void set(hal_dio_value_t value);
    hal_dio_value_t get();

private:
    hal_env_t* m_env;
    hal_handle_t m_handle;
};

class digital_input {
public:
    digital_input(hal_env_t* m_env, hal_port_t port);
    digital_input(digital_input& other) = delete;
    digital_input(digital_input&& other) noexcept;
    digital_input(hal_env_t* env, hal_handle_t handle);
    ~digital_input();

    digital_input& operator=(digital_input& other) = delete;
    digital_input& operator=(digital_input&& other) noexcept;

    hal_dio_value_t get();

private:
    hal_env_t* m_env;
    hal_handle_t m_handle;
};

}
