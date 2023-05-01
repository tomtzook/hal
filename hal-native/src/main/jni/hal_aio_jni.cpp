#include <jni.h>
#include <jnikit.h>
#include <hal.h>
#include <hal_aio.h>

#include "hal_except.h"


extern "C"
JNIEXPORT jint JNICALL Java_hal_HALAIOJNI_get
        (JNIEnv* env, jclass obj, jlong ptr, jlong handle) {
    return jnikit::context<jint>(env, [ptr, handle](jnikit::Env& env) -> jint {
        auto hal_env = reinterpret_cast<hal_env_t*>(ptr);
        hal_aio_value_t value;
        CHECK_ERROR(env, hal_aio_get(hal_env, static_cast<hal_handle_t>(handle), &value));
        return static_cast<jint>(value);
    });
}

extern "C"
JNIEXPORT void JNICALL Java_hal_HALAIOJNI_set
        (JNIEnv* env, jclass obj, jlong ptr, jlong handle, jint value) {
    jnikit::context<void>(env, [ptr, handle, value](jnikit::Env& env) -> void {
        auto hal_env = reinterpret_cast<hal_env_t*>(ptr);
        CHECK_ERROR(env, hal_aio_set(hal_env, static_cast<hal_handle_t>(handle), static_cast<hal_aio_value_t>(value)));
    });
}
