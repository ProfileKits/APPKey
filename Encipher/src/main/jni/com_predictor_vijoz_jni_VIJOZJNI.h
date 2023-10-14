#include <jni.h>
#ifndef _Included_com_predictor_vijoz_jni_VIJOZJNI
#define _Included_com_predictor_vijoz_jni_VIJOZJNI
#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jstring JNICALL Java_com_predictor_vijoz_jni_VIJOZJNI_getKey(
        JNIEnv *,jobject,jobject);

JNIEXPORT jboolean JNICALL Java_com_predictor_vijoz_jni_VIJOZJNI_getPermission(
        JNIEnv *,jobject);

JNIEXPORT jboolean JNICALL Java_com_predictor_vijoz_jni_VIJOZJNI_getToken(
        JNIEnv *,jobject,jobject,jstring,jstring,jint,jint);

JNIEXPORT jstring JNICALL Java_com_predictor_vijoz_jni_VIJOZJNI_getStartCode(
          JNIEnv *,jobject);

JNIEXPORT void JNICALL Java_com_predictor_vijoz_jni_VIJOZJNI_exit(
          JNIEnv *,jobject);

#ifdef __cplusplus
}
#endif
#endif