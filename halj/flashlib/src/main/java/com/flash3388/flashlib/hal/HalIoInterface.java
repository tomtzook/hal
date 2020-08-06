package com.flash3388.flashlib.hal;

import com.flash3388.flashlib.robot.io.AnalogInput;
import com.flash3388.flashlib.robot.io.AnalogOutput;
import com.flash3388.flashlib.robot.io.DigitalInput;
import com.flash3388.flashlib.robot.io.DigitalOutput;
import com.flash3388.flashlib.robot.io.IoChannel;
import com.flash3388.flashlib.robot.io.IoInterface;
import com.flash3388.flashlib.robot.io.Pwm;
import com.hal.Hal;
import com.hal.PortDirection;

public class HalIoInterface implements IoInterface {

    private final Hal mHal;

    public HalIoInterface(Hal hal) {
        mHal = hal;
    }

    @Override
    public AnalogInput newAnalogInput(IoChannel channel) {
        HalIoChannel halIoChannel = IoChannel.cast(channel, HalIoChannel.class);
        int handle = mHal.aioInit(halIoChannel.getPortId(), PortDirection.INPUT);
        return new HalAnalogInput(mHal, handle);
    }

    @Override
    public AnalogOutput newAnalogOutput(IoChannel channel) {
        HalIoChannel halIoChannel = IoChannel.cast(channel, HalIoChannel.class);
        int handle = mHal.aioInit(halIoChannel.getPortId(), PortDirection.OUTPUT);
        return new HalAnalogOutput(mHal, handle);
    }

    @Override
    public DigitalInput newDigitalInput(IoChannel channel) {
        HalIoChannel halIoChannel = IoChannel.cast(channel, HalIoChannel.class);
        int handle = mHal.dioInit(halIoChannel.getPortId(), PortDirection.INPUT);
        return new HalDigitalInput(mHal, handle);
    }

    @Override
    public DigitalOutput newDigitalOutput(IoChannel channel) {
        HalIoChannel halIoChannel = IoChannel.cast(channel, HalIoChannel.class);
        int handle = mHal.dioInit(halIoChannel.getPortId(), PortDirection.OUTPUT);
        return new HalDigitalOutput(mHal, handle);
    }

    @Override
    public Pwm newPwm(IoChannel channel) {
        throw new UnsupportedOperationException("No support in HAL");
    }
}
