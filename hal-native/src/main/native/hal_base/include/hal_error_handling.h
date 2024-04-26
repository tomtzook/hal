#pragma once

#include <errno.h>
#include <syslog.h>
#include <hal_error.h>

#define TRACE(level, format, ...) syslog(level, "(%s:%d): " format, __FILE__, __LINE__, ##__VA_ARGS__)
#define TRACE_DEBUG(format, ...) TRACE(LOG_DEBUG, format, ##__VA_ARGS__)
#define TRACE_INFO(format, ...) TRACE(LOG_INFO, format, ##__VA_ARGS__)
#define TRACE_ERROR(format, ...) TRACE(LOG_ERR, format, ##__VA_ARGS__)
#define TRACE_HAL_ERROR(error_code) TRACE_ERROR("HAL Error (%d) %s", error_code, hal_strerror(error_code))
#define TRACE_SYSTEM_ERROR() TRACE_ERROR("System Error (%d) %s", errno, strerror(errno))

#define HAL_JUMP_IF_ERROR(error_code, label) \
    do { \
         if (HAL_IS_ERROR(error_code)) {   \
            TRACE_HAL_ERROR(error_code);   \
            goto label;                    \
         }  \
    } while(0)

#define HAL_RETURN_IF_ERROR(...) \
    do {                         \
         hal_error_t _status = __VA_ARGS__;  \
         if (HAL_IS_ERROR(_status)) {        \
            TRACE_HAL_ERROR(_status);     \
            return _status;                  \
        }\
    } while(0)

#define HAL_CHECK_ALLOCATED(ptr) \
    do {                            \
        if(NULL == ptr) {        \
            TRACE_ERROR("Allocation failed");   \
            return HAL_ERROR_ALLOCATION_FAILED; \
        } \
    } while(0)

#define HAL_IS_INITIALIZED(ptr) __sync_fetch_and_or(&ptr->initialized, 0)

#define HAL_CHECK_INITIALIZED(ptr) \
    do {                            \
        if(NULL == ptr || !HAL_IS_INITIALIZED(ptr)) { \
            TRACE_ERROR("HAL Uninitialized");     \
            return HAL_ERROR_NOT_INITIALIZED; \
        } \
    } while(0)

#define HAL_CHECK_INITIALIZED_VOID(ptr) \
    do {                            \
        if(NULL == ptr || !HAL_IS_INITIALIZED(ptr)) {  \
            TRACE_ERROR("HAL Uninitialized"); \
            return; \
        } \
    } while(0)
