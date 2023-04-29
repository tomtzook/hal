#pragma once

#include <hal_types.h>
#include <hal_error.h>
#include <hal.h>
#include "halpp_except.h"


namespace hal {

class env;

class generic_port;

template<hal_port_type_t>
class port;


class generic_port {
public:
    virtual ~generic_port();

    generic_port(generic_port& other) = delete;
    generic_port(generic_port&& other) = default;
    generic_port& operator=(generic_port& other) = delete;
    generic_port& operator=(generic_port&& other) = default;

protected:
    hal_env_t* m_env;
    hal_handle_t m_handle;

    explicit generic_port(hal_env_t* env, hal_handle_t handle)
        : m_env(env), m_handle(handle) {}
};

template<>
class port<HAL_TYPE_DIGITAL_INPUT> : public generic_port {
public:
    port(port& other) = delete;
    port(port&& other) = default;
    port& operator=(port& other) = delete;
    port& operator=(port&& other) = default;

    bool get();

private:
    explicit port(hal_env_t* env, hal_handle_t handle)
        : generic_port(env, handle) {}

    friend env;
};

template<>
class port<HAL_TYPE_DIGITAL_OUTPUT> : public generic_port {
public:
    port(port& other) = delete;
    port(port&& other) = default;
    port& operator=(port& other) = delete;
    port& operator=(port&& other) = default;

    bool get();
    void set(bool high);

private:
    explicit port(hal_env_t* env, hal_handle_t handle)
        : generic_port(env, handle) {}

    friend env;
};

}
