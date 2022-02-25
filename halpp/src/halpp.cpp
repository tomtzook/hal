#include "halpp.hpp"

namespace hal {

hal::hal()
    : m_env(init())
    , m_ports(m_env) {
}
hal::~hal() {
    hal_quit(&m_env);
}

ports_sys& hal::ports() {
    return m_ports;
}

hal_env_t* hal::init() {
    hal_env_t* env;
    HAL_CHECK(hal_init(&env));

    return env;
}

}
