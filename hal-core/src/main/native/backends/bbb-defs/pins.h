#pragma once

#define PIN_ID(header, number) (header | number)
#define GPIO_NUMBER(pin) (((pin)->module_index) * 32 + ((pin)->pin_index))

#define PIN_HEADER_P8_ID (0x1 << 6)
#define PIN_HEADER_P9_ID (0x1 << 7)
#define PIN_HEADER_USR_ID (0x3 << 6)

#define P8_13 PIN_ID(PIN_HEADER_P8_ID, 13)
#define P8_14 PIN_ID(PIN_HEADER_P8_ID, 14)
#define P8_17 PIN_ID(PIN_HEADER_P8_ID, 17)
#define P8_19 PIN_ID(PIN_HEADER_P8_ID, 19)
#define P8_31 PIN_ID(PIN_HEADER_P8_ID, 31)
#define P8_32 PIN_ID(PIN_HEADER_P8_ID, 32)
#define P8_33 PIN_ID(PIN_HEADER_P8_ID, 33)
#define P8_35 PIN_ID(PIN_HEADER_P8_ID, 35)
#define P9_11 PIN_ID(PIN_HEADER_P9_ID, 11)
#define P9_13 PIN_ID(PIN_HEADER_P9_ID, 13)
#define P9_17 PIN_ID(PIN_HEADER_P9_ID, 17)
#define P9_18 PIN_ID(PIN_HEADER_P9_ID, 18)
#define P9_19 PIN_ID(PIN_HEADER_P9_ID, 19)
#define P9_20 PIN_ID(PIN_HEADER_P9_ID, 20)
#define P9_21 PIN_ID(PIN_HEADER_P9_ID, 21)
#define P9_22 PIN_ID(PIN_HEADER_P9_ID, 22)
#define P9_24 PIN_ID(PIN_HEADER_P9_ID, 24)
#define P9_26 PIN_ID(PIN_HEADER_P9_ID, 26)
#define P9_41A PIN_ID(PIN_HEADER_P9_ID, 60)
#define P9_42A PIN_ID(PIN_HEADER_P9_ID, 61)
#define P8_03 PIN_ID(PIN_HEADER_P8_ID, 3)
#define P8_04 PIN_ID(PIN_HEADER_P8_ID, 4)
#define P8_05 PIN_ID(PIN_HEADER_P8_ID, 5)
#define P8_06 PIN_ID(PIN_HEADER_P8_ID, 6)
#define P8_11 PIN_ID(PIN_HEADER_P8_ID, 11)
#define P8_12 PIN_ID(PIN_HEADER_P8_ID, 12)
#define P8_15 PIN_ID(PIN_HEADER_P8_ID, 15)
#define P8_16 PIN_ID(PIN_HEADER_P8_ID, 16)
#define P8_20 PIN_ID(PIN_HEADER_P8_ID, 20)
#define P8_21 PIN_ID(PIN_HEADER_P8_ID, 21)
#define P8_22 PIN_ID(PIN_HEADER_P8_ID, 22)
#define P8_23 PIN_ID(PIN_HEADER_P8_ID, 23)
#define P8_24 PIN_ID(PIN_HEADER_P8_ID, 24)
#define P8_25 PIN_ID(PIN_HEADER_P8_ID, 25)
#define P8_26 PIN_ID(PIN_HEADER_P8_ID, 26)
#define P9_12 PIN_ID(PIN_HEADER_P9_ID, 12)
#define P9_14 PIN_ID(PIN_HEADER_P9_ID, 14)
#define P9_15 PIN_ID(PIN_HEADER_P9_ID, 15)
#define P9_16 PIN_ID(PIN_HEADER_P9_ID, 16)
#define P9_23 PIN_ID(PIN_HEADER_P9_ID, 23)
#define USR0 PIN_ID(PIN_HEADER_USR_ID, 0)
#define USR1 PIN_ID(PIN_HEADER_USR_ID, 1)
#define USR2 PIN_ID(PIN_HEADER_USR_ID, 2)
#define USR3 PIN_ID(PIN_HEADER_USR_ID, 3)
#define P8_07 PIN_ID(PIN_HEADER_P8_ID, 7)
#define P8_08 PIN_ID(PIN_HEADER_P8_ID, 8)
#define P8_09 PIN_ID(PIN_HEADER_P8_ID, 9)
#define P8_10 PIN_ID(PIN_HEADER_P8_ID, 10)
#define P8_18 PIN_ID(PIN_HEADER_P8_ID, 18)
#define P8_27 PIN_ID(PIN_HEADER_P8_ID, 27)
#define P8_28 PIN_ID(PIN_HEADER_P8_ID, 28)
#define P8_29 PIN_ID(PIN_HEADER_P8_ID, 29)
#define P8_30 PIN_ID(PIN_HEADER_P8_ID, 30)
#define P8_34 PIN_ID(PIN_HEADER_P8_ID, 34)
#define P8_36 PIN_ID(PIN_HEADER_P8_ID, 36)
#define P8_37 PIN_ID(PIN_HEADER_P8_ID, 37)
#define P8_38 PIN_ID(PIN_HEADER_P8_ID, 38)
#define P8_39 PIN_ID(PIN_HEADER_P8_ID, 39)
#define P8_40 PIN_ID(PIN_HEADER_P8_ID, 40)
#define P8_41 PIN_ID(PIN_HEADER_P8_ID, 41)
#define P8_42 PIN_ID(PIN_HEADER_P8_ID, 42)
#define P8_43 PIN_ID(PIN_HEADER_P8_ID, 43)
#define P8_44 PIN_ID(PIN_HEADER_P8_ID, 44)
#define P8_45 PIN_ID(PIN_HEADER_P8_ID, 45)
#define P8_46 PIN_ID(PIN_HEADER_P8_ID, 46)
#define P9_25 PIN_ID(PIN_HEADER_P9_ID, 25)
#define P9_27 PIN_ID(PIN_HEADER_P9_ID, 27)
#define P9_28 PIN_ID(PIN_HEADER_P9_ID, 28)
#define P9_29 PIN_ID(PIN_HEADER_P9_ID, 29)
#define P9_30 PIN_ID(PIN_HEADER_P9_ID, 30)
#define P9_31 PIN_ID(PIN_HEADER_P9_ID, 31)
#define P9_41B PIN_ID(PIN_HEADER_P9_ID, 62)
#define P9_42B PIN_ID(PIN_HEADER_P9_ID, 63)
#define P9_33 PIN_ID(PIN_HEADER_P9_ID, 33)
#define P9_35 PIN_ID(PIN_HEADER_P9_ID, 35)
#define P9_36 PIN_ID(PIN_HEADER_P9_ID, 36)
#define P9_37 PIN_ID(PIN_HEADER_P9_ID, 37)
#define P9_38 PIN_ID(PIN_HEADER_P9_ID, 38)
#define P9_39 PIN_ID(PIN_HEADER_P9_ID, 39)
#define P9_40 PIN_ID(PIN_HEADER_P9_ID, 40)

#define EHRPWM0A P9_22
#define EHRPWM0B P9_21
#define EHRPWM1A P9_14
#define EHRPWM1B P9_16
#define EHRPWM1A_2 P8_36
#define EHRPWM1B_2 P8_34
#define EHRPWM2A P8_14
#define EHRPWM2B P8_13
#define EHRPWM2A_2 P8_45
#define EHRPWM2B_2 P8_46

#define AIN0 P9_39
#define AIN1 P9_40
#define AIN2 P9_37
#define AIN3 P9_38
#define AIN4 P9_33
#define AIN5 P9_36
#define AIN6 P9_35

#define PIN_TYPE_GPIO (1 << 0)
#define PIN_TYPE_EHRPWM (1 << 1)
#define PIN_TYPE_AIN (1 << 2)

typedef struct {
    const char* name;
    const unsigned id;
    const unsigned module_index;
    const unsigned pin_index;
} pin_t;

typedef struct {
    const unsigned id;
    const unsigned cm_offset;
    const unsigned modes[8];
} pinmux_t;

typedef struct {
    const char* name;
    const unsigned index;
    const unsigned addr;
} pwm_chip_t;

typedef struct {
    const char* name;
    const unsigned id;
    const unsigned chip_index;
    const unsigned index_in_chip;
    const unsigned addr;
} pwm_pin_t;

const pin_t* get_pin(unsigned index);
unsigned get_pin_count();

typedef int(*pin_predicate)(const pin_t*);
const pin_t* find_pin(pin_predicate predicate);
const pin_t* find_pin_by_id(unsigned id);

const pinmux_t* get_pinmux_for_pin(const pin_t* pin);
