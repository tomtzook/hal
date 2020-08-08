package com.hal;

public class HalJni {

    private HalJni() {}

    public static native long initialize();
    public static native void clean(long env);
}
