package hal;

import java.io.Closeable;

public class Port implements Closeable {

    protected final long mEnvPtr;
    protected final long mHandle;

    Port(long envPtr, long handle) {
        mEnvPtr = envPtr;
        mHandle = handle;
    }

    public void setProperty(int key, int value) {
        HALJNI.setProperty(mEnvPtr, mHandle, key, value);
    }

    @Override
    public void close() {
        HALJNI.close(mEnvPtr, mHandle);
    }
}
