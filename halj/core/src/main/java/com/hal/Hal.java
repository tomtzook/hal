package com.hal;

public class Hal implements AutoCloseable {

    public static final int INVALID_HANDLE = -1;

    private long mEnvPointer;

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
    public void dioPulse(int portHandle, long lengthUs) {
        int result = DioJni.pulse(mEnvPointer, portHandle, lengthUs);
        HalResult.fromReturnCode(result).throwIfError();
    }

    public int aioInit(long portId, PortDirection direction) {
        int result = AioJni.initialize(mEnvPointer, portId, direction.directionValue());
        HalResult.fromReturnCode(result).throwIfError();

        return result;
    }
    public void aioFree(int portHandle) {
        int result = AioJni.free(mEnvPointer, portHandle);
        HalResult.fromReturnCode(result).throwIfError();
    }
    public void aioSet(int portHandle, long value) {
        int result = AioJni.set(mEnvPointer, portHandle, value);
        HalResult.fromReturnCode(result).throwIfError();
    }
    public long aioGet(int portHandle) {
        long result = AioJni.get(mEnvPointer, portHandle);
        HalResult.fromReturnCode(result).throwIfError();
        return result;
    }

    @Override
    public void close() {
        HalJni.clean(mEnvPointer);
        mEnvPointer = 0;
    }
}
