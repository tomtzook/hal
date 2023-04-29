package hal;

public class HALJNI {

    private HALJNI() {};

    public static native long init();
    public static native void shutdown(long ptr);

    public static native boolean probe(long ptr, int port, int types);
    public static native long open(long ptr, int port, int type);
    public static native void close(long ptr, long handle);
}
