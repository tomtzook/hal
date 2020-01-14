package com.flash3388.flashlib.hal;

import com.flash3388.flashlib.robot.io.DigitalInput;
import com.hal.Hal;

public class HalDigitalInput extends HalPort implements DigitalInput {

    public HalDigitalInput(Hal hal, int handle) {
        super(hal, handle);
    }

    @Override
    public boolean get() {
        return mHal.dioGet(mHandle);
    }
}
