package com.hal;

public class HalException extends RuntimeException {

    public HalException(int errorCode) {
        super(String.format("HAL error %d", errorCode));
    }
}
