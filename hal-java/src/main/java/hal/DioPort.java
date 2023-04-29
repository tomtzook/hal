package hal;

public class DioPort extends Port {

    DioPort(long envPtr, long handle) {
        super(envPtr, handle);
    }

    public void set(boolean high) {
        HALDIOJNI.set(mEnvPtr, mHandle, high);
    }

    public boolean get() {
        return HALDIOJNI.get(mEnvPtr, mHandle);
    }
}
