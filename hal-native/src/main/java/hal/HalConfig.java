package hal;

public class HalConfig {

    private HalConfig() {}

    public static final int KEY_GPIO_POLL_EDGE = 1;
    public static final int KEY_GPIO_RESISTOR = 2;
    public static final int KEY_ANALOG_MAX_VALUE = 3;
    public static final int KEY_ANALOG_MAX_VOLTAGE = 4;

    public static final int VALUE_GPIO_POLL_EDGE_NONE = 0;
    public static final int VALUE_GPIO_POLL_EDGE_RISING = 1;
    public static final int VALUE_GPIO_POLL_EDGE_FALLING = 2;
    public static final int VALUE_GPIO_POLL_EDGE_BOTH = 3;

    public static final int VALUE_GPIO_RESISTOR_NONE = 0;
    public static final int VALUE_GPIO_RESISTOR_PULLUP = 1;
    public static final int VALUE_GPIO_RESISTOR_PULLDOWN = 2;
}
