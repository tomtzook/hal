package com.flash3388.flashlib.hal;

import com.flash3388.flashlib.robot.io.AnalogOutput;
import com.hal.Hal;

public class HalAnalogOutput extends HalPort implements AnalogOutput {

    public HalAnalogOutput(Hal hal, int handle) {
        super(hal, handle);
    }

    @Override
    public int getValue() {
        return (int) (mHal.aioGet(mHandle) - mHal.aioMinValue());
    }
    @Override
    public void setValue(int value) {
        long rawValue = mHal.aioMinValue() + value;
        mHal.aioSet(mHandle, rawValue);
    }

    @Override
    public double getVoltage() {
        throw new UnsupportedOperationException();
    }
    @Override
    public void setVoltage(double voltage) {
        throw new UnsupportedOperationException();
    }
}
