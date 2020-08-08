#ifndef JNI_HAL_DEFS_H
#define JNI_HAL_DEFS_H

#define PORT_ID(__ID)\
    (port_id_t)__ID

#define PORT_DIR(__DIR)\
    __DIR ? PORT_DIR_OUTPUT : PORT_DIR_INPUT

#define HANDLE(__HANDLE)\
    (hal_handle_t)__HANDLE


#endif //JNI_HAL_DEFS_H
