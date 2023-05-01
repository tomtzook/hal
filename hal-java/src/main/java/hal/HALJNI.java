package hal;

public class HALJNI {

    private HALJNI() {};

    public static native long init();
    public static native void shutdown(long ptr);

    public static native boolean probe(long ptr, String port, int types);
    public static native long open(long ptr, String port, int type);
    public static native void close(long ptr, long handle);

    public static native void setProperty(long ptr, long handle, int key, int value);
}
