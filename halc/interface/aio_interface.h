#ifndef HAL_AIO_INTERFACE_H
#define HAL_AIO_INTERFACE_H

#include "interface.h"

#include "hal_types.h"
#include "hal_aio.h"

hal_result_t aio_port_init(interface_env_t* env, port_id_t port_id, port_dir_t port_dir);
void aio_port_free(interface_env_t* env, port_id_t port_id);

hal_result_t aio_port_write(interface_env_t* env, port_id_t port_id, aio_value_t value);
hal_result_t aio_port_read(interface_env_t* env, port_id_t port_id, aio_value_t* value);

#endif //HAL_AIO_INTERFACE_H
