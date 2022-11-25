#pragma once

#include <type_traits>

#include <hal_types.h>
#include <hal_error.h>
#include <hal.h>
#include "halpp_except.h"


namespace hal {

class env;
class generic_port;
template<hal_port_type_t>
class port ;

env init();



class generic_port {
public:
    virtual ~generic_port();

protected:
    hal_env_t* m_env;
    hal_handle_t m_handle;

    explicit generic_port(hal_env_t* env, hal_handle_t handle)
        : m_env(env)
        , m_handle(handle)
    {}
};

template<>
class port<HAL_TYPE_DIGITAL_INPUT> : public generic_port {
public:

    bool get();

private:
    explicit port(hal_env_t* env, hal_handle_t handle)
        : generic_port(env, handle)
    {}

    friend env;
};

template<>
class port<HAL_TYPE_DIGITAL_OUTPUT> : public generic_port {
public:

    bool get();
    void set(bool high);

private:
    explicit port(hal_env_t* env, hal_handle_t handle)
            : generic_port(env, handle)
    {}

    friend env;
};

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
