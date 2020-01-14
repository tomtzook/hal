#include <stdlib.h>
#include <jni.h>

#include <hal_dio.h>

#define PORT_ID(__ID)\
    (port_id_t)__ID

#define PORT_DIR(__DIR)\
    __DIR ? OUTPUT : INPUT

#define HANDLE(__HANDLE)\
    (hal_handle_t)__HANDLE

#define VALUE(__VALUE)\
    __VALUE ? HIGH : LOW

JNIEXPORT jint JNICALL Java_com_hal_DioJni_initialize
        (JNIEnv *env, jclass obj, jlong env_ptr, jlong port_id, jboolean port_dir){
    hal_env_t* hal_env = (hal_env_t*) env_ptr;

    hal_handle_t handle;

    hal_result_t result = hal_dio_init(hal_env, PORT_ID(port_id), PORT_DIR(port_dir), &handle);
    if (HAL_SUCCESS != result) {
        return -result;
    }

    return (jint)handle;
}

JNIEXPORT jint JNICALL Java_com_hal_DioJni_free
        (JNIEnv *env, jclass obj, jlong env_ptr, jint handle){
    hal_env_t* hal_env = (hal_env_t*) env_ptr;

    hal_result_t result = hal_dio_free(hal_env, HANDLE(handle));
    if (HAL_SUCCESS != result) {
        return -result;
    }

    return 0;
}

JNIEXPORT jint JNICALL Java_com_hal_DioJni_set
        (JNIEnv *env, jclass obj, jlong env_ptr, jint handle, jboolean value){
    hal_env_t* hal_env = (hal_env_t*) env_ptr;

    hal_result_t result = hal_dio_set(hal_env, HANDLE(handle), VALUE(value));
    if (HAL_SUCCESS != result) {
        return -result;
    }

    return 0;
}

JNIEXPORT jint JNICALL Java_com_hal_DioJni_get
        (JNIEnv *env, jclass obj, jlong env_ptr, jint handle){
    hal_env_t* hal_env = (hal_env_t*) env_ptr;

    dio_value_t value;

    hal_result_t result = hal_dio_get(hal_env, HANDLE(handle), &value);
    if (HAL_SUCCESS != result) {
        return -result;
    }

    return value == HIGH ? 1 : 0;
}
