#pragma once

#include <stdint.h>


#define GPIO0_PERIPHERAL_BASE 0x44e07000
#define GPIO1_PERIPHERAL_BASE 0x4804c000
#define GPIO2_PERIPHERAL_BASE 0x481ac000
#define GPIO3_PERIPHERAL_BASE 0x481ae000
#define GPIO_PERIPHERAL_SIZE 0x1000

#define CM_PERIPHERAL_BASE 0x44e10000
#define CM_PERIPHERAL_SIZE 0x2000

#pragma pack(push, 1)

typedef struct _gpio_registers {
    typedef enum _idle_mode : uint32_t {
        idle_mode_force = 0x0,
        idle_mode_no = 0x1,
        idle_mode_smart = 0x2,
        idle_mode_smart_wakeup = 0x3
    } idle_mode_t;

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
            idle_mode_t idle_mode : 2;
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
} gpio_registers_t;

typedef struct _cm_registers {
    struct {
        uint32_t ip_rev_minor : 6;
        uint32_t ip_rev_custom : 2;
        uint32_t ip_rev_major : 3;
        uint32_t ip_rev_rtl : 5;
        uint32_t ip_rev_func : 12;
        uint32_t reserved : 2;
        uint32_t ip_rev_scheme : 2;
    } control_revision;
    uint32_t ip_hwinfo;
    uint8_t reserved0[8];
    struct {
        uint32_t reserved0 : 1;
        uint32_t freemu : 1;
        uint32_t idlemode : 2;
        uint32_t standby : 2;
        uint32_t reserved1 : 26;
    } control_sysconfig;
    uint8_t reserved1[0x2c];
    struct {
        uint32_t sysboot0 : 8;
        uint32_t devtype : 3;
        uint32_t reserved0 : 5;
        uint32_t bw : 1;
        uint32_t waiten : 1;
        uint32_t admux : 2;
        uint32_t testmd : 2;
        uint32_t sysboot1 : 2;
        uint32_t reserved1 : 7;
    } control_status;
    uint8_t reserved2[0xcc];
    struct {
        uint32_t pagesize : 3;
        uint32_t ebank : 1;
        uint32_t ibank : 3;
        uint32_t rowsize : 3;
        uint32_t cl : 4;
        uint32_t narrow_mode : 2;
        uint32_t cwl : 2;
        uint32_t sdram_drive : 2;
        uint32_t reserved0 : 1;
        uint32_t dyn_odt : 2;
        uint32_t ddr2_ddqs : 1;
        uint32_t ddr_term : 3;
        uint32_t ibank_pos : 2;
        uint32_t sdram_type : 3;
    } control_emif_sdram_config;
    uint8_t reserved3[0x314];
    struct {
        uint32_t reserved0 : 16;
        uint32_t vset : 10;
        uint32_t reserved1 : 6;
    } core_sldo_ctrl;
    struct {
        uint32_t reserved0 : 16;
        uint32_t vset : 10;
        uint32_t reserved1 : 6;
    } mpu_sldo_ctrl;
    uint8_t reserved4[0x14];
    struct {
        uint32_t clkdivopp50_en : 1;
        uint32_t reserved0 : 31;
    } clk32divratio_ctrl;
    struct {
        uint32_t tshut : 1;
        uint32_t ecoz : 1;
        uint32_t contcov : 1;
        uint32_t clrz : 1;
        uint32_t soc : 1;
        uint32_t tempsoff : 1;
        uint32_t bgroff : 1;
        uint32_t cbiassel : 1;
        uint32_t dtemp : 8;
        uint32_t reserved0 : 16;
    } bandgap_ctrl;
    struct {
        uint32_t dtrtempsc : 8;
        uint32_t dtrtemps : 8;
        uint32_t dtrbgapv : 8;
        uint32_t dtrbgapc : 8;
    } bandgap_trim;
    uint8_t reserved5[0x8];
    struct {
        uint32_t mpu_dpll_clkinpulow_set : 1;
        uint32_t disp_pll_clkinplulow_sel : 1;
        uint32_t ddr_pll_clkinpulow_sel : 1;
        uint32_t reserved0 : 29;
    } pll_clkinplulow;
    uint8_t reserved6[0xc];
    struct {
        uint32_t resselect : 1;
        uint32_t reserved0 : 31;
    } mosc_ctrl;
    uint8_t reserved7[0x4];
    struct {
        uint32_t dscount : 16;
        uint32_t reserved0 : 1;
        uint32_t dsenable : 1;
        uint32_t reserved1 : 14;
    } deepsleep_ctrl;
    uint8_t reserved8[0x98];
    struct {
        uint32_t reserved0 : 8;
        uint32_t ponout_per : 1;
        uint32_t pgoodout_per : 1;
        uint32_t reserved1 : 6;
        uint32_t ponout_disp : 1;
        uint32_t pgoodout_disp : 1;
        uint32_t reserved2 : 6;
        uint32_t ponout_ddr : 1;
        uint32_t pgoodout_ddr : 1;
        uint32_t reserved3 : 6;
    } dpll_pwr_sw_status;
    uint8_t reserved9[0xf0];
    struct {
        uint32_t reserved0 : 1;
        uint32_t mgfr : 11;
        uint32_t partnum : 16;
        uint32_t devrev : 4;
    } device_id;
    struct {
        uint32_t dev_feature_bits : 32;
    } dev_feature;
    struct {
        uint32_t host_arm : 2;
        uint32_t reserved0 : 2;
        uint32_t pru_icss : 2;
        uint32_t mmu : 2;
        uint32_t reserved1 : 6;
        uint32_t p1500 : 2;
        uint32_t tcrd0 : 2;
        uint32_t tcwr0 : 2;
        uint32_t tcrd1 : 2;
        uint32_t tcwr1 : 2;
        uint32_t tcrd2 : 2;
        uint32_t tcwr2 : 2;
        uint32_t reserved2 : 4;
    } init_priority_0;
    struct {
        uint32_t cpsw : 2;
        uint32_t reserved0 : 2;
        uint32_t usb_dma : 2;
        uint32_t usb_qmgr : 2;
        uint32_t reserved1 : 8;
        uint32_t reserved2 : 2;
        uint32_t reserved3 : 2;
        uint32_t sgx : 2;
        uint32_t lcd : 2;
        uint32_t debug : 2;
        uint32_t reserved4 : 6;
    } init_priority_1;
    uint8_t reserved10[0x4];
    struct {
        uint32_t tc0dbs : 2;
        uint32_t tc1dbs : 2;
        uint32_t tc2dbs : 2;
        uint32_t reserved0 : 26;
    } tptc_cfg;
    uint8_t reserved11[0x8];
    struct {
        uint32_t cm_pwrdn : 1;
        uint32_t otg_pwrdn : 1;
        uint32_t chgdet_dis : 1;
        uint32_t chgdet_rsrt : 1;
        uint32_t srcondm : 1;
        uint32_t sinkondp : 1;
        uint32_t chgisink_en : 1;
        uint32_t chgvsrv_en : 1;
        uint32_t dmpulldn : 1;
        uint32_t dppullup : 1;
        uint32_t cdet_extcl : 1;
        uint32_t reserved0 : 1;
        uint32_t gpiomode : 1;
        uint32_t gpio_sig_inv : 1;
        uint32_t gpio_sig_cross : 1;
        uint32_t reserved1 : 1;
        uint32_t reserved2 : 1;
        uint32_t dpgpio_pd : 1;
        uint32_t dmgpio_pd : 1;
        uint32_t otgvdet_en : 1;
        uint32_t otgsessenden : 1;
        uint32_t reserved3 : 1;
        uint32_t reserved4 : 1;
        uint32_t datapolarity_inv : 1;
        uint32_t reserved5 : 8;
    } usb_ctrl0;
} cm_registers_t;

#pragma pack(pop)
