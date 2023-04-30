
#include <hal.h>
#include <hal_error.h>

#include "hal_except.h"


void throw_hal_error(jnikit::Env& env, hal_error_t error_code) {
    auto str_error = hal_strerror(error_code);
    env.throwException<HalException>(str_error);
}