package com.flash3388.flashlib.hal;

import com.flash3388.flashlib.robot.io.AnalogAccumulator;
import com.flash3388.flashlib.robot.io.AnalogInput;
import com.hal.Hal;

public class HalAnalogInput extends HalPort implements AnalogInput {

    public HalAnalogInput(Hal hal, int handle) {
        super(hal, handle);
    }

    @Override
    public int getValue() {
        return (int) (mHal.aioGet(mHandle) - mHal.aioMinValue());
    }

    @Override
    public double getVoltage() {
        throw new UnsupportedOperationException();
    }

    @Override
    public double getMaxVoltage() {
        throw new UnsupportedOperationException();
    }

    @Override
    public int getMaxValue() {
        return (int) (mHal.aioMaxValue() - mHal.aioMinValue());
    }

    @Override
    public AnalogAccumulator getAccumulator() {
        throw new UnsupportedOperationException();
    }

    @Override
    public double getSampleRate() {
        throw new UnsupportedOperationException();
    }
}
