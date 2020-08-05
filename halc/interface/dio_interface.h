#ifndef HAL_DIO_INTERFACE_H
#define HAL_DIO_INTERFACE_H

#include "interface.h"

#include "hal_types.h"
#include "hal_dio.h"

int dio_port_init(interface_env_t* env, port_id_t port_id, port_dir_t port_dir);
void dio_port_free(interface_env_t* env, port_id_t port_id);

void dio_port_write(interface_env_t* env, port_id_t port_id, dio_value_t value);
dio_value_t dio_port_read(interface_env_t* env, port_id_t port_id);
void dio_port_pulse(interface_env_t* env, port_id_t port_id, uint64_t length_us);

#endif //HAL_DIO_INTERFACE_H
