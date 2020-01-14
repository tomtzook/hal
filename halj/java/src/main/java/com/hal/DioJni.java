package com.hal;

public class DioJni {

    private DioJni() {}

    public static native int initialize(long env, long portId, boolean portDir);
    public static native int free(long env, int portHandle);

    public static native int set(long env, int portHandle, boolean value);
    public static native int get(long env, int portHandle);

    public static boolean intToDioValue(int value) {
        return value > 0;
    }
}
