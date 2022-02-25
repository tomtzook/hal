#pragma once

#include <exception>
#include <hal_error.h>

namespace hal {

class exception : std::exception {
public:
    explicit exception(hal_error_t error) : m_error(error) {}

    hal_error_t error() const { return m_error; }

    virtual const char* what() const noexcept override {
        return "HAL error";
    }

private:
    hal_error_t m_error;
};

}

#define HAL_CHECK(...) \
    do {               \
        hal_error_t __status = __VA_ARGS__; \
        if (HAL_IS_ERROR(__status)) {       \
            throw ::hal::exception(__status);               \
        }                   \
    } while(0)
