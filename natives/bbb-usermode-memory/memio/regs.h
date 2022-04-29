#pragma once

#include <stdint.h>


#define REG(peripheral, register) ((volatile register*)(peripheral->base))

#pragma pack(push, 1)

// gpio_registers

#define GPIO0 0x44e07000
#define GPIO1 0x4804c000
#define GPIO2 0x481ac000
#define GPIO3 0x481ae000
#define GPIO_CTRL_SIZE 0x1000

typedef enum {
    idle_mode_force = 0x0,
    idle_mode_no = 0x1,
    idle_mode_smart = 0x2,
    idle_mode_smart_wakeup = 0x3
} gpio_sysconfig_idle_mode_t;

typedef struct {
    volatile union {
        volatile uint32_t data;
        volatile struct {
            uint32_t minor : 6;
            uint32_t custom : 2;
            uint32_t major : 3;
            uint32_t rtl : 5;
            uint32_t func : 12;
            uint32_t reserved0 : 2;
            uint32_t scheme : 2;
        } bits;
    } revision;
    uint8_t reserved0[0xc];
    volatile union {
        volatile uint32_t data;
        volatile struct {
            uint32_t auto_idle : 1;
            uint32_t softreset : 1;
            uint32_t ena_wakeup : 1;
            gpio_sysconfig_idle_mode_t idle_mode : 2;
        } bits;
    } sysconfig;
    uint8_t reserved1[0xc];
    volatile union {
        volatile uint32_t data;
        volatile struct {
            uint32_t dma_event_ack : 1;
        } bits;
    } eoi;
    volatile uint32_t irqstatus_raw0;
    volatile uint32_t irqstatus_raw1;
    volatile uint32_t irqstatus0;
    volatile uint32_t irqstatus1;
    volatile uint32_t irqstatus_set0;
    volatile uint32_t irqstatus_set1;
    volatile uint32_t irqstatus_clr0;
    volatile uint32_t irqstatus_clr1;
    volatile uint32_t irqwaken0;
    volatile uint32_t irqwaken1;
    uint8_t reserved2[0xc8];
    volatile uint32_t sysstatus;
    uint8_t reserved3[0x18];
    volatile union {
        volatile uint32_t data;
        volatile struct {
            uint32_t disable_module : 1;
            uint32_t gating_ratio : 2;
        } bits;
    } ctrl;
    volatile uint32_t oe;
    volatile uint32_t datain;
    volatile uint32_t dataout;
    volatile uint32_t leveldetect0;
    volatile uint32_t leveldetect1;
    volatile uint32_t risingdetect;
    volatile uint32_t fallingdetect;
    volatile uint32_t debouncingenable;
    volatile union {
        volatile uint32_t data;
        volatile struct {
            uint32_t debouncingtime : 8;
        } bits;
    } debouncingtime;
    uint8_t reserved5[0x38];
    volatile uint32_t cleardataout;
    volatile uint32_t setdataout;
} gpio_registers;

#pragma pack(pop)
