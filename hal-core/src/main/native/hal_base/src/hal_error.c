
#include "hal_error.h"


const char* hal_strerror(hal_error_t error) {
    switch (error) {
        case HAL_SUCCESS: return "Success";
        case HAL_ERROR_NOT_INITIALIZED: return "HAL environment not initialized";
        case HAL_ERROR_ALLOCATION_FAILED: return "HAL failed to allocate memory";
        case HAL_ERROR_NOT_FOUND: return "HAL could not find resource";
        case HAL_ERROR_UNSUPPORTED_OPERATION: return "HAL operation not supported";
        case HAL_ERROR_TAKEN: return "HAL resource already taken";
        case HAL_ERROR_BAD_DATA: return "HAL value is corrupted";
        case HAL_ERROR_BAD_ARGUMENT: return "HAL bad argument given";
        case HAL_ERROR_PERMISSIONS: return "HAL missing system permissions";
        case HAL_ERROR_BAD_HANDLE: return "HAL was given a bad handle";
        case HAL_ERROR_TYPE_NOT_SUPPORTED: return "HAL type is not supported by port";
        case HAL_ERROR_ENVIRONMENT: return "HAL encountered system error";
        case HAL_ERROR_CONFIG_KEY_NOT_SUPPORTED: return "HAL configuration key is not supported";
        case HAL_ERROR_OPERATION_NOT_SUPPORTED_FOR_TYPE: return "HAL operation not supported for port type";
        case HAL_ERROR_ITER_END: return "HAL iteration reached its end";
        case HAL_ERROR_CONFIG_KEY_NOT_READABLE: return "HAL config key is not readable";
        case HAL_ERROR_CONFIG_KEY_NOT_WRITABLE: return "HAL config key is not writable";
        default: return "N/A";
    }
}
