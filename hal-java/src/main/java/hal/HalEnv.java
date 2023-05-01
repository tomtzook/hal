package hal;

import java.io.Closeable;
import java.io.IOException;

public class HalEnv implements Closeable {

    private static final int HAL_TYPE_DIGITAL_INPUT = (0x1 << 0);
    private static final int HAL_TYPE_DIGITAL_OUTPUT = (0x1 << 1);
    private static final int HAL_TYPE_ANALOG_INPUT = (0x1 << 2);
    private static final int HAL_TYPE_ANALOG_OUTPUT = (0x1 << 3);

    private final long mEnvPtr;

    HalEnv(long envPtr) {
        mEnvPtr = envPtr;
    }

    public DioPort openDigitalPort(String port, boolean output) {
        long handle = HALJNI.open(mEnvPtr, port,
                output ? HAL_TYPE_DIGITAL_OUTPUT : HAL_TYPE_DIGITAL_INPUT);
        return new DioPort(mEnvPtr, handle);
    }

    public AioPort openAnalogPort(String port, boolean output) {
        long handle = HALJNI.open(mEnvPtr, port,
                output ? HAL_TYPE_ANALOG_OUTPUT : HAL_TYPE_ANALOG_INPUT);
        return new AioPort(mEnvPtr, handle);
    }

    @Override
    public void close() throws IOException {
        HALJNI.shutdown(mEnvPtr);
    }
}
