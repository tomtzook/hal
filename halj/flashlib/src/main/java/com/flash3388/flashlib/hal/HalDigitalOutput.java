package com.flash3388.flashlib.hal;

import com.flash3388.flashlib.robot.io.DigitalOutput;
import com.hal.Hal;

public class HalDigitalOutput extends HalPort implements DigitalOutput {

    public HalDigitalOutput(Hal hal, int handle) {
        super(hal, handle);
    }

    @Override
    public boolean get() {
        return mHal.dioGet(mHandle);
    }

    @Override
    public void set(boolean high) {
        mHal.dioSet(mHandle, high);
    }

    @Override
    public void pulse(double length) {
        throw new UnsupportedOperationException("Pulse not supported by HAL");
    }
}
