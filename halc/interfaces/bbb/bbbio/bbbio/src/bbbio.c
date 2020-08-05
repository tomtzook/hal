#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <zconf.h>
#include <sys/mman.h>

#include <bbbio_debug.h>
#include <bbbio_rc.h>
#include <bbbio_consts.h>
#include <bbbio.h>

#include "gpio/bbbio_gpio_consts.h"
#include "pwmss/bbbio_pwmss_consts.h"
#include "pwmss/bbbio_pwmss_ctrl.h"
#include "bbbio_util.h"
#include "hw.h"


#define GOTO_ON_NO_SUCCESS(value, error_goto, error_msg) \
	if (SUCCESS != value) { \
		BBBIO_DEBUG(error_msg); \
		goto error_goto; \
	}

#define ERROR_MAP_FAILED(ptr, error_goto, return_code_var, return_code, error_msg, error_args...) \
	if (MAP_FAILED == ptr) { \
		BBBIO_DEBUGF(error_msg, error_args); \
		return_code_var = return_code; \
		goto error_goto; \
	}

static bbbio_rc_t bbbio_gpio_init(bbbio_t* bbbio) {
	bbbio_rc_t return_code = SUCCESS;

	for (size_t i = 0; i < BBBIO_GPIO_MODULES_COUNT; ++i) {
		bbbio->gpio_module_addr[i] = mmap(0, BBBIO_GPIO_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, bbbio->mem_fd, bbbio_gpio_addr_offset[i]);
		ERROR_MAP_FAILED(bbbio->gpio_module_addr[i], free_gpio, return_code, EMAP, "BBBIO failed to map gpio %zu: %s \n", i, strerror(errno));
	}

free_gpio:
	return return_code;
}

static void bbbio_gpio_free(bbbio_t* bbbio) {
	for (size_t i = 0; i < BBBIO_GPIO_MODULES_COUNT; ++i) {
		MUNMAP(bbbio->gpio_module_addr[i], BBBIO_GPIO_LEN);
	}
}

static bbbio_rc_t bbbio_pwmss_init(bbbio_t* bbbio) {
	bbbio_rc_t return_code = SUCCESS;

	for (size_t i = 0; i < BBBIO_PWMSS_MODULES_COUNT; ++i) {
		bbbio->pwmss_module_addr[i] = mmap(0, BBBIO_PWMSS_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, bbbio->mem_fd, bbbio_pwmss_addr_offset[i]);
		ERROR_MAP_FAILED(bbbio->pwmss_module_addr[i], free_pwmss, return_code, EMAP, "BBBIO failed to map pwmss %zu: %s \n", i, strerror(errno));

		bbbio->epwm_module_addr[i] = HWADD(bbbio->pwmss_module_addr[i], BBBIO_PWMSS_EPWM_OFFSET);
        return_code = bbbio_module_clk_enable(bbbio, i);
        GOTO_ON_NO_SUCCESS(return_code, free_pwmss, "BBBIO failed to enable PWMSS module clock");
	}

free_pwmss:
	return return_code;
}

static void bbbio_pwmss_free(bbbio_t* bbbio) {
	for (size_t i = 0; i < BBBIO_PWMSS_MODULES_COUNT; ++i) {
		MUNMAP(bbbio->pwmss_module_addr[i], BBBIO_PWMSS_LEN);
		bbbio->epwm_module_addr[i] = 0;
	}
}

bbbio_rc_t bbbio_init(bbbio_t* bbbio) {
	if (bbbio->mem_fd) {
		BBBIO_DEBUG("BBBIO already initialized \n");
		return EINITIALIZED;
	}

	bbbio->mem_fd = open("/dev/mem", O_RDWR);
	if (bbbio->mem_fd < 0) {
	    BBBIO_DEBUGF("BBBIO failed to map /dev/mem: %s \n", strerror(errno));
        return EMAP;
	}

	bbbio_rc_t return_code;

	bbbio->cm_per_addr = mmap(0, BBBIO_CM_PER_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, bbbio->mem_fd, BBBIO_CM_PER_ADDR);
	ERROR_MAP_FAILED(bbbio->cm_per_addr, free_init, return_code, EMAP, "BBBIO failed to map control module per address: %s \n", strerror(errno));

	bbbio->cm_addr = mmap(0, BBBIO_CONTROL_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, bbbio->mem_fd, BBBIO_CONTROL_MODULE);
	ERROR_MAP_FAILED(bbbio->cm_addr, free_init, return_code, EMAP, "BBBIO failed to map control module: %s \n", strerror(errno));

	return_code = bbbio_gpio_init(bbbio);
	GOTO_ON_NO_SUCCESS(return_code, free_init, "BBBIO failed to init GPIO");

	return_code = bbbio_pwmss_init(bbbio);
	GOTO_ON_NO_SUCCESS(return_code, free_init, "BBBIO failed to init PWMSS");

	return SUCCESS;
free_init:
	bbbio_free(bbbio);
	return return_code;
}

void bbbio_free(bbbio_t* bbbio) {
	bbbio_pwmss_free(bbbio);
	bbbio_gpio_free(bbbio);

	MUNMAP(bbbio->cm_addr, BBBIO_CONTROL_LEN);
	MUNMAP(bbbio->cm_per_addr, BBBIO_CM_PER_LEN);

    CLOSE_FD(bbbio->mem_fd);
}
