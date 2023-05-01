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
    port<T> open(const char* port_name) {
        auto handle = open(port_name, T);
        return port<T>{m_env, handle};
    }

private:
    hal_env_t* m_env;

    explicit env(hal_env_t* env)
        : m_env(env)
    {}

    hal_handle_t open(const char* port_name, hal_port_type_t type);

    friend env init();
};

}
