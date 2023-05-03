#pragma once

#include <errno.h>
#include <syslog.h>
#include <hal_error.h>

#define TRACE(level, format, ...) syslog(level, format, ##__VA_ARGS__)
#define TRACE_DEBUG(format, ...) TRACE(LOG_DEBUG, format, ##__VA_ARGS__)
#define TRACE_INFO(format, ...) TRACE(LOG_INFO, format, ##__VA_ARGS__)
#define TRACE_ERROR(format, ...) TRACE(LOG_ERR, "Error (%s:%d)" format, __FILE__, __LINE__, ##__VA_ARGS__)
#define TRACE_SYSTEM_ERROR() TRACE_ERROR(": (%d) %s", errno, strerror(errno))

#define HAL_JUMP_IF_ERROR(error_code, label) \
    do { \
         if (HAL_IS_ERROR(error_code)) {     \
            TRACE_ERROR();                             \
            goto label;                                    \
         }  \
    } while(0)

#define HAL_RETURN_IF_ERROR(...) \
    do {                         \
         hal_error_t _status = __VA_ARGS__;  \
         if (HAL_IS_ERROR(_status)) {        \
            TRACE_ERROR();                     \
            return _status;      \
        }\
    } while(0)

#define HAL_CHECK_ALLOCATED(ptr) \
    do {                            \
        if(NULL == ptr) {        \
            TRACE_ERROR("Allocation failed");                     \
            return HAL_ERROR_ALLOCATION_FAILED; \
        } \
    } while(0)

#define HAL_CHECK_INITIALIZED(ptr) \
    do {                            \
        if(NULL == ptr || !ptr->initialized) { \
            TRACE_ERROR("Uninitialized");                       \
            return HAL_ERROR_NOT_INITIALIZED; \
        } \
    } while(0)

#define HAL_CHECK_INITIALIZED_VOID(ptr) \
    do {                            \
        if(NULL == ptr || !ptr->initialized) {               \
            TRACE_ERROR("Uninitialized");\
            return; \
        } \
    } while(0)
