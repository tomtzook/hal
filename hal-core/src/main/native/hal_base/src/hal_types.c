#include <hal_types.h>


const char* hal_port_type_str(const hal_port_type_t type) {
    switch (type) {
        case HAL_TYPE_DIGITAL_INPUT: return "HAL_TYPE_DIGITAL_INPUT";
        case HAL_TYPE_DIGITAL_OUTPUT: return "HAL_TYPE_DIGITAL_OUTPUT";
        case HAL_TYPE_ANALOG_INPUT: return "HAL_TYPE_ANALOG_INPUT";
        case HAL_TYPE_ANALOG_OUTPUT: return "HAL_TYPE_ANALOG_OUTPUT";
        case HAL_TYPE_PWM_OUTPUT: return "HAL_TYPE_PWM_OUTPUT";
        case HAL_TYPE_QUADRATURE: return "HAL_TYPE_QUADRATURE";
        default: return "N/A";
    }
}

const char* hal_port_flag_str(const hal_port_flag_t flag) {
    switch (flag) {
        case HAL_FLAG_OPEN: return "HAL_FLAG_OPEN";
        case HAL_FLAG_BLOCKED: return "HAL_FLAG_BLOCKED";
        default: return "N/A";
    }
}

const char* hal_prop_key_str(const hal_prop_key_t key) {
    switch (key) {
        case HAL_CONFIG_DIO_POLL_EDGE: return "HAL_CONFIG_DIO_POLL_EDGE";
        case HAL_CONFIG_DIO_RESISTOR: return "HAL_CONFIG_DIO_RESISTOR";
        case HAL_CONFIG_ANALOG_MAX_VALUE: return "HAL_CONFIG_ANALOG_MAX_VALUE";
        case HAL_CONFIG_ANALOG_MAX_VOLTAGE: return "HAL_CONFIG_ANALOG_MAX_VOLTAGE";
        case HAL_CONFIG_ANALOG_SAMPLE_RATE: return "HAL_CONFIG_ANALOG_SAMPLE_RATE";
        case HAL_CONFIG_PWM_FREQUENCY: return "HAL_CONFIG_PWM_FREQUENCY";
        default: return "N/A";
    }
}

const char* hal_prop_flag_str(const uint32_t flag) {
    switch (flag) {
        case HAL_CONFIG_FLAG_READABLE: return "HAL_CONFIG_FLAG_READABLE";
        case HAL_CONFIG_FLAG_WRITABLE: return "HAL_CONFIG_FLAG_WRITABLE";
        default: return "N/A";
    }
}
