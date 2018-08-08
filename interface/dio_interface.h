#ifndef HAL_DIO_INTERFACE_H
#define HAL_DIO_INTERFACE_H

#include <hal_types.h>
#include <hal_dio.h>

int dio_init();
void dio_free();

int dio_port_init(port_id_t port_id, port_dir_t port_dir);
void dio_port_free(port_id_t port_id);

void dio_port_write(port_id_t port_id, dio_value_t value);
dio_value_t dio_port_read(port_id_t port_id);

#endif //HAL_DIO_INTERFACE_H
