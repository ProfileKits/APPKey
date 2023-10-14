LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := VIJOZ
#为了使用__android_log_print()在JNI中打印log要设置这个属性
LOCAL_LDLIBS := -llog
LOCAL_SRC_FILES := com_predictor_vijoz_jni_VIJOZJNI.cpp

include $(BUILD_SHARED_LIBRARY)