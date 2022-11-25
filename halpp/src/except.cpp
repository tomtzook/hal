
#include "halpp_except.h"


namespace hal {

void throw_if_error(hal_error_t error_code) {
    switch (error_code) {
        case HAL_SUCCESS:
            return;
        case HAL_ERROR_NOT_INITIALIZED:
            throw hal_not_initialized();
        case HAL_ERROR_ALLOCATION_FAILED:
            throw hal_allocation_error();
        case HAL_ERROR_NOT_FOUND:
            throw hal_not_found();
        case HAL_ERROR_UNSUPPORTED_OPERATION:
            throw hal_unsupported_operation();
        case HAL_ERROR_TAKEN:
            throw hal_already_taken();
        case HAL_ERROR_BAD_DATA:
            throw hal_bad_data();
        case HAL_ERROR_BAD_ARGUMENT:
            throw hal_bad_argument();
        case HAL_ERROR_PERMISSIONS:
            throw hal_permission_error();
        case HAL_ERROR_BAD_HANDLE:
            throw hal_bad_handle();
        case HAL_ERROR_TYPE_NOT_SUPPORTED:
            throw hal_type_not_supported();
        default:
            throw hal_general_exception(error_code);
    }
}

}

