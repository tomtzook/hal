package com.hal;

public class Hal implements AutoCloseable {

    private final long mEnvPointer;

    private Hal(long envPointer) {
        mEnvPointer = envPointer;
    }

    public static Hal initialize() {
        long envPtr = HalJni.initialize();
        HalResult.fromEnvReturnCode(envPtr).throwIfError();

        return new Hal(envPtr);
    }

    public int dioInit(long portId, PortDirection direction) {
        int result = DioJni.initialize(mEnvPointer, portId, direction.directionValue());
        HalResult.fromReturnCode(result).throwIfError();

        return result;
    }

    public void dioFree(int portHandle) {
        int result = DioJni.free(mEnvPointer, portHandle);
        HalResult.fromReturnCode(result).throwIfError();
    }

    public void dioSet(int portHandle, boolean high) {
        int result = DioJni.set(mEnvPointer, portHandle, high);
        HalResult.fromReturnCode(result).throwIfError();
    }

    public boolean dioGet(int portHandle) {
        int result = DioJni.get(mEnvPointer, portHandle);
        HalResult.fromReturnCode(result).throwIfError();

        return DioJni.intToDioValue(result);
    }

    @Override
    public void close() {
        HalJni.clean(mEnvPointer);
    }
}
