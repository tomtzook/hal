#include <stdlib.h>
#include <jni.h>

#include <hal.h>

JNIEXPORT jlong JNICALL Java_com_hal_HalJni_initialize
  (JNIEnv *env, jclass obj) {
    hal_env_t* hal_env = NULL;

    hal_result_t result = hal_initialize(&hal_env);
    if (HAL_SUCCESS != result) {
        return -((jlong) result);
    }

    return (jlong)hal_env;
}

JNIEXPORT void JNICALL Java_com_hal_HalJni_clean
        (JNIEnv *env, jclass obj, jlong env_ptr) {
    hal_env_t* hal_env = (hal_env_t*) env_ptr;
    hal_clean(&hal_env);
}