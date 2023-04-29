package hal;

import java.io.Closeable;

public class Port implements Closeable {

    protected final long mEnvPtr;
    protected final long mHandle;

    Port(long envPtr, long handle) {
        mEnvPtr = envPtr;
        mHandle = handle;
    }

    @Override
    public void close() {
        HALJNI.close(mEnvPtr, mHandle);
    }
}
