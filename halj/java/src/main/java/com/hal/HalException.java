package com.hal;

public class HalException extends RuntimeException {

    public HalException(String errorName, int errorCode) {
        super(String.format("HAL error (%d) %s", errorCode, errorName));
    }
}
