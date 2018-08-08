#ifndef HAL_DIO_H
#define HAL_DIO_H

#include "hal_types.h"

typedef enum {
    HIGH,
    LOW
} dio_value_t;

hal_result_t hal_dio_init_module();
void hal_dio_free_module();

hal_result_t hal_dio_init(port_id_t port_id, port_dir_t port_dir, hal_handle_t* result);
hal_result_t hal_dio_free(hal_handle_t hal_handle);

hal_result_t hal_dio_set(hal_handle_t hal_handle, dio_value_t dio_value);
hal_result_t hal_dio_get(hal_handle_t hal_handle, dio_value_t* result);

#endif //HAL_DIO_H
