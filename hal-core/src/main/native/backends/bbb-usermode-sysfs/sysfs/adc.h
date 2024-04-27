#pragma once

#include <hal_types.h>
#include <hal_error.h>
#include <hal_aio.h>


hal_error_t adc_read(unsigned number, uint32_t* value);
