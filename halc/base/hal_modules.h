#ifndef HAL_HAL_MODULES_H
#define HAL_HAL_MODULES_H

hal_result_t hal_dio_init_module(hal_env_t* env);
void hal_dio_free_module(hal_env_t* env);

hal_result_t hal_aio_init_module(hal_env_t* env);
void hal_aio_free_module(hal_env_t* env);

#endif //HAL_HAL_MODULES_H
