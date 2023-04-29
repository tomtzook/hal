#pragma once

#include <exception>
#include "hal_error.h"


namespace hal {

class hal_exception : public std::exception {

};

class hal_general_exception : public std::exception {
public:
    hal_general_exception(hal_error_t error_code)
        : m_error_code(error_code)
    {}

    [[nodiscard]] hal_error_t error_code() const {
        return m_error_code;
    }

private:
    hal_error_t m_error_code;
};

class hal_not_initialized : public hal_general_exception {
public:
    hal_not_initialized()
        : hal_general_exception(HAL_ERROR_NOT_INITIALIZED)
    {}
};

class hal_allocation_error : public hal_general_exception {
public:
    hal_allocation_error()
        : hal_general_exception(HAL_ERROR_ALLOCATION_FAILED)
    {}
};

class hal_not_found : public hal_general_exception {
public:
    hal_not_found()
        : hal_general_exception(HAL_ERROR_NOT_FOUND)
    {}
};

class hal_unsupported_operation : public hal_general_exception {
public:
    hal_unsupported_operation()
        : hal_general_exception(HAL_ERROR_UNSUPPORTED_OPERATION)
    {}
};

class hal_already_taken : public hal_general_exception {
public:
    hal_already_taken()
        : hal_general_exception(HAL_ERROR_TAKEN)
    {}
};

class hal_bad_data : public hal_general_exception {
public:
    hal_bad_data()
        : hal_general_exception(HAL_ERROR_BAD_DATA)
    {}
};

class hal_bad_argument : public hal_general_exception {
public:
    hal_bad_argument()
        : hal_general_exception(HAL_ERROR_BAD_ARGUMENT)
    {}
};

class hal_permission_error : public hal_general_exception {
public:
    hal_permission_error()
        : hal_general_exception(HAL_ERROR_PERMISSIONS)
    {}
};

class hal_bad_handle : public hal_general_exception {
public:
    hal_bad_handle()
        : hal_general_exception(HAL_ERROR_BAD_HANDLE)
    {}
};

class hal_type_not_supported : public hal_general_exception {
public:
    hal_type_not_supported()
        : hal_general_exception(HAL_ERROR_TYPE_NOT_SUPPORTED)
    {}
};

void throw_if_error(hal_error_t error_code);

}

#define HAL_CALL(...) \
    do {              \
        auto __status = __VA_ARGS__; \
        hal::throw_if_error(__status);\
    } while(0)
