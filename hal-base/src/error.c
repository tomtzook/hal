
#include <hal_error.h>


const char* hal_strerror(hal_error_t error) {
    switch (error) {
        case HAL_SUCCESS: return "Success";
        case HAL_ERROR_NOT_INITIALIZED: return "Not Initialized";
        case HAL_ERROR_NOT_ALLOCATED: return "Allocation Error";
        case HAL_ERROR_NOT_FOUND: return "Not Found";
        case HAL_ERROR_UNSUPPORTED_OPERATION: return "Operation Not Supported";
        case HAL_ERROR_TAKEN: return "Already Taken";
        case HAL_ERROR_BAD_DATA: return "Bad Data";
        case HAL_ERROR_BAD_ARGUMENT: return "Bad Argument";
        case HAL_ERROR_PERMISSIONS: return "No Permissions";
        default: return "Unknown";
    }
}
