#include <jni.h>
#include <jnikit.h>
#include <hal.h>

#include "hal_except.h"


extern "C"
JNIEXPORT jlong JNICALL Java_hal_HALJNI_init
        (JNIEnv* env, jclass obj) {
    return jnikit::context<jlong>(env, [](jnikit::Env& env) -> jlong {
        hal_env_t* hal_env;
        CHECK_ERROR(env, hal_init(&hal_env));
        return reinterpret_cast<jlong>(hal_env);
    });
}

extern "C"
JNIEXPORT void JNICALL Java_hal_HALJNI_shutdown
        (JNIEnv* env, jclass obj, jlong ptr) {
    jnikit::context<void>(env, [ptr](jnikit::Env& env) -> void {
        auto hal_env = reinterpret_cast<hal_env_t*>(ptr);
        hal_shutdown(hal_env);
    });
}

extern "C"
JNIEXPORT jboolean JNICALL Java_hal_HALJNI_probe
        (JNIEnv* _env, jclass obj, jlong ptr, jstring port, jint types) {
    return jnikit::context<jboolean>(_env, [_env, ptr, port, types](jnikit::Env& env) -> jboolean {
        auto hal_env = reinterpret_cast<hal_env_t*>(ptr);

        const char* port_name = _env->GetStringUTFChars(port, nullptr);
        auto status = hal_probe(hal_env, port_name, static_cast<hal_port_type_t>(types));
        _env->ReleaseStringUTFChars(port, port_name);

        if (status == HAL_SUCCESS) {
            return true;
        }
        if (status == HAL_ERROR_TYPE_NOT_SUPPORTED) {
            return false;
        }

        CHECK_ERROR2(env, status);
        return false;
    });
}

extern "C"
JNIEXPORT jlong JNICALL Java_hal_HALJNI_open
        (JNIEnv* _env, jclass obj, jlong ptr, jstring port, jint type) {
    return jnikit::context<jlong>(_env, [_env, ptr, port, type](jnikit::Env& env) -> jlong {
        auto hal_env = reinterpret_cast<hal_env_t*>(ptr);

        const char* port_name = _env->GetStringUTFChars(port, nullptr);

        hal_handle_t handle;
        auto status = hal_open(hal_env, port_name, static_cast<hal_port_type_t>(type), &handle);
        _env->ReleaseStringUTFChars(port, port_name);
        CHECK_ERROR2(env, status);

        return static_cast<jlong>(handle);
    });
}

extern "C"
JNIEXPORT void JNICALL Java_hal_HALJNI_close
        (JNIEnv* env, jclass obj, jlong ptr, jlong handle) {
    jnikit::context<void>(env, [ptr, handle](jnikit::Env& env) -> void {
        auto hal_env = reinterpret_cast<hal_env_t*>(ptr);
        hal_close(hal_env, static_cast<hal_handle_t>(handle));
    });
}

