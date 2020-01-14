package com.flash3388.flashlib.hal;

import com.flash3388.flashlib.robot.io.IoChannel;

public class HalIoChannel implements IoChannel {

    private final long mPortId;

    public HalIoChannel(long portId) {
        mPortId = portId;
    }

    public long getPortId() {
        return mPortId;
    }
}
