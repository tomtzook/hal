#include <jni.h>
#include <jnikit.h>
#include <hal.h>
#include <hal_dio.h>

#include "hal_except.h"


extern "C"
JNIEXPORT jboolean JNICALL Java_hal_HALDIOJNI_get
        (JNIEnv* env, jclass obj, jlong ptr, jlong handle) {
    jnikit::context<jboolean>(env, [ptr, handle](jnikit::Env& env) -> jboolean {
        auto hal_env = reinterpret_cast<hal_env_t*>(ptr);
        hal_dio_value_t value;
        CHECK_ERROR(env, hal_dio_get(hal_env, static_cast<hal_handle_t>(handle), &value));
        return value == HAL_DIO_HIGH;
    });
}

extern "C"
JNIEXPORT void JNICALL Java_hal_HALDIOJNI_set
        (JNIEnv* env, jclass obj, jlong ptr, jlong handle, jboolean high) {
    jnikit::context<bool>(env, [ptr, handle, high](jnikit::Env& env) -> bool {
        auto hal_env = reinterpret_cast<hal_env_t*>(ptr);
        CHECK_ERROR(env, hal_dio_set(hal_env, static_cast<hal_handle_t>(handle),
                                     high ? HAL_DIO_HIGH : HAL_DIO_LOW));
    });
}