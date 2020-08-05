package com.hal;

public enum HalResult {
    HAL_SUCCESS(0),
    HAL_NOT_INITIALIZED(1),
    HAL_ARGUMENT_ERROR(2),
    HAL_PORT_NOT_INITIALIZED(3),
    HAL_IO_ERROR(4),
    HAL_STORE_ERROR(5),
    HAL_MEMORY_ALLOCATION_ERROR(6),
    HAL_INITIALIZATION_ERROR(7),
    HAL_ALREADY_INITIALIZED(8),

    HAL_UNKNOWN_ERROR(1000)
    ;

    private final int mErrorCode;

    HalResult(int errorCode) {
        mErrorCode = errorCode;
    }

    public int errorCode() {
        return mErrorCode;
    }

    public void throwIfError() {
        if (mErrorCode != HAL_SUCCESS.mErrorCode) {
            throw new HalException(name(), mErrorCode);
        }
    }

    public static HalResult fromReturnCode(int result) {
        if (result >= 0) {
            return HAL_SUCCESS;
        }

        return fromErrorCode(-result);
    }

    public static HalResult fromEnvReturnCode(long result) {
        if (result > 0) {
            return HAL_SUCCESS;
        } else if (result == 0) {
            throw new EnumConstantNotPresentException(HalResult.class, "Env error 0");
        }

        return fromErrorCode((int) -result);
    }

    private static HalResult fromErrorCode(int errorCode) {
        for (HalResult halResult : values()) {
            if (halResult.errorCode() == errorCode) {
                return halResult;
            }
        }

        throw new EnumConstantNotPresentException(HalResult.class, String.valueOf(errorCode));
    }
}
