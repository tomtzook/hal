package com.hal;

public enum PortDirection {
    OUTPUT(true),
    INPUT(false)
    ;

    private final boolean mDirectionValue;

    PortDirection(boolean directionValue) {
        mDirectionValue = directionValue;
    }

    public boolean directionValue() {
        return mDirectionValue;
    }
}
