#pragma once

#include <hal_error.h>
#include <jnikit.h>
#include <except.h>


#define CHECK_ERROR(__env, __VA_ARGS__) \
do {                             \
    auto __rc = __VA_ARGS__;     \
    CHECK_ERROR2(__env, __rc);   \
} while(0)

#define CHECK_ERROR2(__env, __rc) \
do {                             \
    if (HAL_IS_ERROR(__rc)) {              \
        throw_hal_error(__env, __rc);   \
    }                                 \
} while(0)


void throw_hal_error(jnikit::Env& env, hal_error_t error_code);
