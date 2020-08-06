#include <stdlib.h>
#include <jni.h>

#include <hal_aio.h>

#include "hal_defs.h"

#define VALUE(__VALUE)\
    (aio_value_t) (__VALUE)

#define VALUE_JAVA(__VALUE)\
    (jlong) (__VALUE)

JNIEXPORT jint JNICALL Java_com_hal_AioJni_initialize
        (JNIEnv *env, jclass obj, jlong env_ptr, jlong port_id, jboolean port_dir) {
    hal_env_t* hal_env = (hal_env_t*) env_ptr;

    hal_handle_t handle;

    hal_result_t result = hal_aio_init(hal_env, PORT_ID(port_id), PORT_DIR(port_dir), &handle);
    if (HAL_SUCCESS != result) {
        return -((jint) result);
    }

    return (jint)handle;
}

JNIEXPORT jint JNICALL Java_com_hal_AioJni_free
        (JNIEnv *env, jclass obj, jlong env_ptr, jint handle) {
    hal_env_t* hal_env = (hal_env_t*) env_ptr;

    hal_result_t result = hal_aio_free(hal_env, HANDLE(handle));
    if (HAL_SUCCESS != result) {
        return -((jint) result);
    }

    return 0;
}

JNIEXPORT jint JNICALL Java_com_hal_AioJni_set
        (JNIEnv *env, jclass obj, jlong env_ptr, jint handle, jlong value) {
    hal_env_t* hal_env = (hal_env_t*) env_ptr;

    hal_result_t result = hal_aio_set(hal_env, HANDLE(handle), VALUE(value));
    if (HAL_SUCCESS != result) {
        return -((jint) result);
    }

    return 0;
}

JNIEXPORT jlong JNICALL Java_com_hal_AioJni_get
        (JNIEnv *env, jclass obj, jlong env_ptr, jint handle) {
    hal_env_t* hal_env = (hal_env_t*) env_ptr;

    aio_value_t value;

    hal_result_t result = hal_aio_get(hal_env, HANDLE(handle), &value);
    if (HAL_SUCCESS != result) {
        return -((jlong) result);
    }

    return VALUE_JAVA(value);
}
