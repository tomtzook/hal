package com.hal;

public class Hal implements AutoCloseable {

    private Hal() {

    }

    public static Hal initialize() {
        return new Hal();
    }

    public native int dioInitialize();

    @Override
    public void close() throws Exception {

    }
}
