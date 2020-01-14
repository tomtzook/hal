package com.hal;

public enum HalError {
    HAL_SUCCESS(0),
    HAL_NOT_INITIALIZED(1),
    HAL_ARGUMENT_ERROR(2),
    HAL_PORT_NOT_INITIALIZED(3),
    HAL_IO_ERROR(4),
    HAL_STORE_ERROR(5),
    HAL_MEMORY_ALLOCATION_ERROR(6),
    HAL_INITIALIZATION_ERROR(7),
    HAL_ALREADY_INITIALIZED(8),
    HAL_DIO_INITIALIZATION_ERROR(9)
    ;

    private final int mErrorCode;

    HalError(int errorCode) {
        mErrorCode = errorCode;
    }

    public int errorCode() {
        return mErrorCode;
    }
}
