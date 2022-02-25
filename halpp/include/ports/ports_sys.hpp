#pragma once

#include "ports/ports_dio.hpp"
#include "ports/ports_aio.hpp"

namespace hal {

class ports_sys {
public:
    ports_sys(hal_env_t* env) noexcept : m_env(env) {}

    digital_output new_digital_output(hal_port_t port) { return {m_env, port}; }
    digital_input new_digital_input(hal_port_t port) { return {m_env, port}; }

private:
    hal_env_t* m_env;
};

}
