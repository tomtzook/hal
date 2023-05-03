#include <jni.h>
#include <jnikit.h>
#include <hal.h>
#include <hal_pwm.h>

#include "hal_except.h"


extern "C"
JNIEXPORT jfloat JNICALL Java_hal_HALPWMJNI_getDutyCycle
        (JNIEnv* env, jclass obj, jlong ptr, jlong handle) {
    return jnikit::context<jfloat>(env, [ptr, handle](jnikit::Env& env) -> jfloat {
        auto hal_env = reinterpret_cast<hal_env_t*>(ptr);
        float value;
        CHECK_ERROR(env, hal_pwm_get_duty_cycle(hal_env, static_cast<hal_handle_t>(handle), &value));
        return static_cast<jfloat>(value);
    });
}

extern "C"
JNIEXPORT jfloat JNICALL Java_hal_HALPWMJNI_getFrequency
        (JNIEnv* env, jclass obj, jlong ptr, jlong handle) {
    return jnikit::context<jfloat>(env, [ptr, handle](jnikit::Env& env) -> jfloat {
        auto hal_env = reinterpret_cast<hal_env_t*>(ptr);
        float value;
        CHECK_ERROR(env, hal_pwm_get_frequency(hal_env, static_cast<hal_handle_t>(handle), &value));
        return static_cast<jfloat>(value);
    });
}

extern "C"
JNIEXPORT void JNICALL Java_hal_HALPWMJNI_setDutyCycle
        (JNIEnv* env, jclass obj, jlong ptr, jlong handle, jfloat value) {
    jnikit::context<void>(env, [ptr, handle, value](jnikit::Env& env) -> void {
        auto hal_env = reinterpret_cast<hal_env_t*>(ptr);
        CHECK_ERROR(env, hal_pwm_set_duty_cycle(hal_env, static_cast<hal_handle_t>(handle),
                                                static_cast<float >(value)));
    });
}

extern "C"
JNIEXPORT void JNICALL Java_hal_HALPWMJNI_setFrequency
        (JNIEnv* env, jclass obj, jlong ptr, jlong handle, jfloat value) {
    jnikit::context<void>(env, [ptr, handle, value](jnikit::Env& env) -> void {
        auto hal_env = reinterpret_cast<hal_env_t*>(ptr);
        CHECK_ERROR(env, hal_pwm_set_frequency(hal_env, static_cast<hal_handle_t>(handle),
                                                static_cast<float >(value)));
    });
}
