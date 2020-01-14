#include <stdlib.h>
#include <jni.h>

#include "mad360.h"

JNIEXPORT jint JNICALL Java_mcl_mad360_Mad360Jni_initHandle
  (JNIEnv *env, jclass obj){
    return M360_InitHandle();
}

JNIEXPORT jint JNICALL Java_mcl_mad360_Mad360Jni_initHandleOrGetExisting
        (JNIEnv *env, jclass obj){
    return M360_InitHandleOrGetExisting();
}

JNIEXPORT jintArray JNICALL Java_mcl_mad360_Mad360Jni_getAllHandles
  (JNIEnv *env, jclass obj) {
    int number_of_handles = M360_NumberOfCurrentHandles();
    int* handles_array = (int*) malloc(numberOfHandles * sizeof(int));
    if (NULL == handlesArray) {
        return NULL;// TODO: HANDLE MEMORY ERROR
    }

    int handles_filled = M360_GetAllHandles(handles_array, number_of_handles);

    jintArray result = (*env)->NewIntArray(env, handles_filled);
    if (NULL == result) {
        free(handles_array);
        return NULL;// TODO: HANDLE MEMORY ERROR
    }

    (*env)->SetIntArrayRegion(env, result, 0, handles_filled, handles_array);
    free(handlesArray);

	return result;
}

JNIEXPORT jint JNICALL Java_mcl_mad360_Mad360Jni_getHandleBySerial
        (JNIEnv *env, jclass obj, jshort serial){
    return M360_GetHandleBySerial(serial);
}

JNIEXPORT jint JNICALL Java_mcl_mad360_Mad360Jni_grabHandle
        (JNIEnv *env, jclass obj, jshort device){
    return M360_GrabHandle(device);
}

JNIEXPORT jint JNICALL Java_mcl_mad360_Mad360Jni_grabHandleOrGetExisting
        (JNIEnv *env, jclass obj, jshort device){
    return M360_GrabHandleOrGetExisting(device);
}

JNIEXPORT jint JNICALL Java_mcl_mad360_Mad360Jni_grabAllHandles
        (JNIEnv *env, jclass obj){
    return M360_GrabAllHandles();
}

JNIEXPORT void JNICALL Java_mcl_mad360_Mad360Jni_releaseHandle
        (JNIEnv *env, jclass obj, jint handle){
    M360_ReleaseHandle(handle);
}

JNIEXPORT void JNICALL Java_mcl_mad360_Mad360Jni_releaseAllHandles
        (JNIEnv *env, jclass obj){
    M360_ReleaseAllHandles();
}

JNIEXPORT jint JNICALL Java_mcl_mad360_Mad360Jni_numberOfCurrentHandles
        (JNIEnv *env, jclass obj){
    return M360_NumberOfCurrentHandles();
}

JNIEXPORT jintArray JNICALL Java_mcl_mad360_Mad360Jni_status
        (JNIEnv *env, jclass obj, jint handle){
    unsigned char status = 0;
    int result = M360_Status(&status, handle);

    jintArray jresult = (*env)->NewIntArray(env, 4);
    if (NULL == jresult) {
        return NULL;// TODO: HANDLE MEMORY ERROR
    }

    jint* body = (*env)->GetIntArrayElements(env, jresult, 0);
    body[0] = result;
    body[1] = status & 1;
    body[2] = status & (1 >> 1);
    body[3] = status & (1 >> 2);
    (*env)->ReleaseIntArrayElements(env, jresult, body, 0);

    return jresult;
}

JNIEXPORT jintArray JNICALL Java_mcl_mad360_Mad360Jni_stop
        (JNIEnv *env, jclass obj, jint handle){
    unsigned char status = 0;
    int result = M360_Stop(&status, handle);

    jintArray jresult = (*env)->NewIntArray(env, 4);
    if (NULL == jresult) {
        return NULL;// TODO: HANDLE MEMORY ERROR
    }

    jint* body = (*env)->GetIntArrayElements(env, jresult, 0);
    body[0] = result;
    body[1] = status & 1;
    body[2] = status & (1 >> 1);
    body[3] = status & (1 >> 2);
    (*env)->ReleaseIntArrayElements(env, jresult, body, 0);

    return jresult;
}

JNIEXPORT jint JNICALL Java_mcl_mad360_Mad360Jni_rotateM
        (JNIEnv *env, jclass obj, jint axis, jdouble velocity, jint micro_steps, jint handle){
    return M360_RotateM(axis, velocity, micro_steps, handle);
}

JNIEXPORT jint JNICALL Java_mcl_mad360_Mad360Jni_rotate
        (JNIEnv *env, jclass obj, jint axis, jdouble velocity, jdouble distance, jint handle){
    return M360_Rotate(axis, velocity, distance, handle);
}

JNIEXPORT jint JNICALL Java_mcl_mad360_Mad360Jni_singleStep
        (JNIEnv *env, jclass obj, jint axis, jint direction, jint handle){
    return M360_SingleStep(axis, direction, handle);
}

JNIEXPORT jintArray JNICALL Java_mcl_mad360_Mad360Jni_currentPositionM
        (JNIEnv *env, jclass obj, jint axis, jint handle){
    int micro_steps = 0;
    int result = M360_CurrentPositionM(axis, &micro_steps, handle);

    jintArray jresult = (*env)->NewIntArray(env, 2);
    if (NULL == jresult) {
        return NULL;// TODO: HANDLE MEMORY ERROR
    }

    jint* body = (*env)->GetIntArrayElements(env, jresult, 0);
    body[0] = result;
    body[1] = micro_steps;
    (*env)->ReleaseIntArrayElements(env, jresult, body, 0);

    return jresult;
}

JNIEXPORT jint JNICALL Java_mcl_mad360_Mad360Jni_findHome
        (JNIEnv *env, jclass obj, jint axis, jint handle){
    return M360_FindHome(axis, handle);
}

JNIEXPORT jintArray JNICALL Java_mcl_mad360_Mad360Jni_moveStatus
        (JNIEnv *env, jclass obj, jint handle){
    int is_moving = 0;
    int result = M360_MoveStatus(&is_moving, handle);

    jintArray jresult = (*env)->NewIntArray(env, 2);
    if (NULL == jresult) {
        return NULL;// TODO: HANDLE MEMORY ERROR
    }

    jint* body = (*env)->GetIntArrayElements(env, jresult, 0);
    body[0] = result;
    body[1] = is_moving;
    (*env)->ReleaseIntArrayElements(env, jresult, body, 0);

    return jresult;
}


JNIEXPORT jint JNICALL Java_mcl_mad360_Mad360Jni_wait
        (JNIEnv *env, jclass obj, jint handle){
    return M360_Wait(handle);
}

JNIEXPORT jintArray JNICALL Java_mcl_mad360_Mad360Jni_getWaitTime
        (JNIEnv *env, jclass obj, jint handle){
    int wait = 0;
    int result = M360_GetWaitTime(&wait, handle);

    jintArray jresult = (*env)->NewIntArray(env, 2);
    if (NULL == jresult) {
        return NULL;// TODO: HANDLE MEMORY ERROR
    }

    jint* body = (*env)->GetIntArrayElements(env, jresult, 0);
    body[0] = result;
    body[1] = wait;
    (*env)->ReleaseIntArrayElements(env, jresult, body, 0);

    return jresult;
}

JNIEXPORT jintArray JNICALL Java_mcl_mad360_Mad360Jni_getRotation
        (JNIEnv *env, jclass obj, jint axis, jint handle){
    int rotations = 0;
    int result = M360_GetRotations(axis, &rotations, handle);

    jintArray jresult = (*env)->NewIntArray(env, 2);
    if (NULL == jresult) {
        return NULL;// TODO: HANDLE MEMORY ERROR
    }

    jint* body = (*env)->GetIntArrayElements(env, jresult, 0);
    body[0] = result;
    body[1] = rotations;
    (*env)->ReleaseIntArrayElements(env, jresult, body, 0);

    return jresult;
}

JNIEXPORT jint JNICALL Java_mcl_mad360_Mad360Jni_getSerialNumber
        (JNIEnv *env, jclass obj, jint handle){
    return M360_GetSerialNumber(handle);
}

JNIEXPORT jintArray JNICALL Java_mcl_mad360_Mad360Jni_getProductID
        (JNIEnv *env, jclass obj, jint handle){
    unsigned short product_id = 0;
    int result = M360_GetProductID(&product_id, handle);

    jintArray jresult = (*env)->NewIntArray(env, 2);
    if (NULL == jresult) {
        return NULL;// TODO: HANDLE MEMORY ERROR
    }

    jint* body = (*env)->GetIntArrayElements(env, jresult, 0);
    body[0] = result;
    body[1] = product_id;
    (*env)->ReleaseIntArrayElements(env, jresult, body, 0);

    return jresult;
}

JNIEXPORT jdoubleArray JNICALL Java_mcl_mad360_Mad360Jni_information
        (JNIEnv *env, jclass obj, jint axis, jint handle){
    double step_size = 0.0;
    double max_velocity = 0.0;
    double max_velocity_two_axis = 0.0;
    double min_velocity = 0.0;
    int result = M360_Information(axis,
                                &step_size,
                                &max_velocity,
                                &max_velocity_two_axis,
                                &min_velocity,
                                handle);

    jdoubleArray jresult = (*env)->NewDoubleArray(env, 5);
    if (NULL == jresult) {
        return NULL;// TODO: HANDLE MEMORY ERROR
    }

    jdouble* body = (*env)->GetDoubleArrayElements(env, jresult, 0);
    body[0] = result;
    body[1] = step_size;
    body[2] = max_velocity;
    body[3] = max_velocity_two_axis;
    body[4] = min_velocity;
    (*env)->ReleaseDoubleArrayElements(env, jresult, body, 0);

    return jresult;
}

JNIEXPORT jintArray JNICALL Java_mcl_mad360_Mad360Jni_getAxisInfo
        (JNIEnv *env, jclass obj, jint handle){
    unsigned short axis_bitmap = 0;
    int result = M360_GetAxisInfo(&axis_bitmap, handle);

    jintArray jresult = (*env)->NewIntArray(env, 3);
    if (NULL == jresult) {
        return NULL;// TODO: HANDLE MEMORY ERROR
    }

    jint* body = (*env)->GetIntArrayElements(env, jresult, 0);
    body[0] = result;
    body[1] = axis_bitmap & 1;
    body[1] = axis_bitmap & (1 >> 1);
    (*env)->ReleaseIntArrayElements(env, jresult, body, 0);

    return jresult;
}