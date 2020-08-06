package com.hal;

public class AioJni {

    private AioJni() {}

    public static native long getMaxValue(long env);
    public static native long getMinValue(long env);

    public static native int initialize(long env, long portId, boolean portDir);
    public static native int free(long env, int portHandle);

    public static native int set(long env, int portHandle, long value);
    public static native long get(long env, int portHandle);
}
