#ifndef HAL_HAL_GLOBAL_H
#define HAL_HAL_GLOBAL_H

extern int hal_initialized;

#define HAL_IS_INITIALIZED() \
    hal_initialized == 1
#define HAL_NOT_INITIALIZED() \
    hal_initialized == 0


#define HAL_SET_INITIALIZED() \
    hal_initialized = 1
#define HAL_SET_NOT_INITIALIZED() \
    hal_initialized = 0

#endif //HAL_HAL_GLOBAL_H
