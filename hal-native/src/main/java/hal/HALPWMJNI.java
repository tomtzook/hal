package hal;

public class HALPWMJNI {

    private HALPWMJNI() {}

    public static native float getDutyCycle(long ptr, long handle);
    public static native float getFrequency(long ptr, long handle);

    public static native void setDutyCycle(long ptr, long handle, float value);
    public static native void setFrequency(long ptr, long handle, float value);
}
