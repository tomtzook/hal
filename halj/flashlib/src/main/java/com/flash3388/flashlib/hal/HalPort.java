package com.flash3388.flashlib.hal;

import com.flash3388.flashlib.robot.io.IoPort;
import com.hal.Hal;

public class HalPort implements IoPort {

    protected final Hal mHal;
    protected int mHandle;

    public HalPort(Hal hal, int handle) {
        mHal = hal;
        mHandle = handle;
    }

    @Override
    public void free() {
        mHal.dioFree(mHandle);
        mHandle = Hal.INVALID_HANDLE;
    }
}
