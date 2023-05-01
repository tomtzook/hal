package hal;

public class HALDIOJNI {

    private HALDIOJNI() {}

    public static native boolean get(long ptr, long handle);
    public static native void set(long ptr, long handle, boolean high);
}
