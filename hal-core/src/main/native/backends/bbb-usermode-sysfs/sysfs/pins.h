#pragma once


#define PIN_NUMBER(module, index) ((module) * 32 + (index))
#define ANALOG_MAX_VALUE 4095
#define ANALOG_MAX_VOLTAGE 3.3f
#define ANALOG_MAX_VOLTAGE_MV 3300
#define ANALOG_SAMPLE_RATE 200000.0f // 200khz
#define ANALOG_SAMPLE_RATE_PERIOD_US 5


typedef struct {
    const char* name;
    unsigned pin_number;
    uint32_t supported_types;
} pin_t;

typedef struct {
    const char* module_name;
    const char* chip;
    const char* addr;
    int chip_index;
    int index;
} pwm_pin_t;

typedef struct {
    const char* name;
    const char* module_name;
} pwm_port_t;


extern pin_t PINS[];

extern pwm_pin_t PWM_PINS[];
extern pwm_port_t PWM_PORTS[];


pin_t* find_pin_def_for_name(const char* port_name);
pin_t* find_pin_def_for_id(hal_id_t id);
pin_t* get_pin_def_for_index(size_t index);
size_t get_pin_def_count();

hal_error_t set_pin_mode(pin_t* pin, const char* mode);
hal_error_t get_pin_mode(pin_t* pin, char* buffer);

const char* get_pwm_module_name_for_pin(pin_t* pin);
pwm_pin_t* get_pwm_pin_for_module(const char* module_name);
