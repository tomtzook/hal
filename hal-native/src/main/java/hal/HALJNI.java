package hal;

public class HALJNI {

    private HALJNI() {};

    public static final int HAL_TYPE_DIGITAL_INPUT = (0x1 << 0);
    public static final int HAL_TYPE_DIGITAL_OUTPUT = (0x1 << 1);
    public static final int HAL_TYPE_ANALOG_INPUT = (0x1 << 2);
    public static final int HAL_TYPE_ANALOG_OUTPUT = (0x1 << 3);

    public static native long init();
    public static native void shutdown(long ptr);

    public static native boolean probe(long ptr, String port, int types);
    public static native long open(long ptr, String port, int type);
    public static native void close(long ptr, long handle);

    public static native int getProperty(long ptr, long handle, int key);
    public static native float getProperty_f(long ptr, long handle, int key);
    public static native void setProperty(long ptr, long handle, int key, int value);
    public static native void setProperty_f(long ptr, long handle, int key, float value);
}
