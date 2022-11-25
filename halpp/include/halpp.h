#pragma once

#include <type_traits>

#include <hal_types.h>
#include <hal_error.h>
#include <hal.h>
#include "halpp_except.h"
#include "halpp_ports.h"


namespace hal {

class env;

env init();

class env {
public:
    env(env&) = delete;
    env(env&&) = delete;
    ~env();

    env& operator=(env&) = delete;
    env& operator=(env&&) = delete;

    template<hal_port_type_t T>
    port<T> open(hal_port_t port);

private:
    hal_env_t* m_env;

    explicit env(hal_env_t* env)
        : m_env(env)
    {}

    hal_handle_t open(hal_port_t port, hal_port_type_t type);

    friend env init();
};

template<hal_port_type_t T>
port<T> env::open(hal_port_t _port) {
    auto handle = open(_port, T);
    return port<T>{m_env, handle};
}

}
