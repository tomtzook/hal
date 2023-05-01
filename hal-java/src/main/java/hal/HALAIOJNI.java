package hal;

public class HALAIOJNI {

    private HALAIOJNI() {}

    public static native int get(long ptr, long handle);
    public static native void set(long ptr, long handle, int value);
}
