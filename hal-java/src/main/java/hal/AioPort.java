package hal;

public class AioPort extends Port {

    AioPort(long envPtr, long handle) {
        super(envPtr, handle);
    }

    public void set(int value) {
        HALAIOJNI.set(mEnvPtr, mHandle, value);
    }

    public int get() {
        return HALAIOJNI.get(mEnvPtr, mHandle);
    }
}
