#ifndef HAL_DIO_INTERFACE_H
#define HAL_DIO_INTERFACE_H

#include "hal_types.h"
#include "hal_dio.h"

typedef struct dio_env dio_env_t;

int dio_init(dio_env_t** env);
void dio_free(dio_env_t** env);

int dio_port_init(dio_env_t* env, port_id_t port_id, port_dir_t port_dir);
void dio_port_free(dio_env_t* env, port_id_t port_id);

void dio_port_write(dio_env_t* env, port_id_t port_id, dio_value_t value);
dio_value_t dio_port_read(dio_env_t* env, port_id_t port_id);

#endif //HAL_DIO_INTERFACE_H
