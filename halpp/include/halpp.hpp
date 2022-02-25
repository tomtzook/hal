#pragma once

#include <hal.h>
#include <halpp_except.hpp>

#include "ports/ports_sys.hpp"

namespace hal {

class hal {
public:
    hal();
    ~hal();

    ports_sys& ports();

private:
    static hal_env_t* init();

    hal_env_t* m_env;
    ports_sys m_ports;
};

}
