#include<jni.h>
#include<string>
#include"valid.cpp"
#include <thread>
#include <chrono>

using namespace
std;
#ifndef _Included_com_predictor_vijoz_jni_VIJOZJNI
#define _Included_com_predictor_vijoz_jni_VIJOZJNI
#ifdef __cplusplus
extern "C"
{
#endif
        jboolean canUse = false;
        jstring char2Jstring (JNIEnv * env, const char*pat )
        {
            //定义java String类 strClass
            jclass strClass = (env) -> FindClass("java/lang/String");
            //获取java String类方法String(byte[],String)的构造器,用于将本地byte[]数组转换为一个新String
            jmethodID ctorID = (env) -> GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
            //建立byte数组
            jbyteArray bytes = (env) -> NewByteArray((jsize) strlen(pat));
            //将char* 转换为byte数组
            (env) -> SetByteArrayRegion(bytes, 0, (jsize) strlen(pat), (jbyte *)pat);
            //设置String, 保存语言类型,用于byte数组转换至String时的参数
            jstring encoding = (env) -> NewStringUTF("GB2312");
            //将byte数组转换为java String,并输出
            return (jstring) (env) -> NewObject(strClass, ctorID, bytes, encoding);
        }


        char*Jstring2CStr(JNIEnv * env, jstring jstr)
        {
            char*rtn = NULL;
            jclass clsstring = env -> FindClass("java/lang/String");
            jstring strencode = env -> NewStringUTF("GB2312");
            jmethodID mid = env -> GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
            jbyteArray barr = (jbyteArray) env -> CallObjectMethod(jstr, mid, strencode);
            jsize alen = env -> GetArrayLength(barr);
            jbyte * ba = env -> GetByteArrayElements(barr, JNI_FALSE);
            if (alen > 0) {
                rtn = ( char*)malloc(alen + 1);//new char[alen+1];
                memcpy(rtn, ba, alen);
                rtn[alen] = 0;
            }
            env -> ReleaseByteArrayElements(barr, ba, 0);
            return rtn;
        }

        char*jstringToChar(JNIEnv * env, jstring jstr) {
        char*rtn = NULL;
        jclass clsstring = env -> FindClass("java/lang/String");
        jstring strencode = env -> NewStringUTF("GB2312");
        jmethodID mid = env -> GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
        jbyteArray barr = (jbyteArray) env -> CallObjectMethod(jstr, mid, strencode);
        jsize alen = env -> GetArrayLength(barr);
        jbyte * ba = env -> GetByteArrayElements(barr, JNI_FALSE);
        if (alen > 0) {
            rtn = ( char*)malloc(alen + 1);
            memcpy(rtn, ba, alen);
            rtn[alen] = 0;
        }
        env -> ReleaseByteArrayElements(barr, ba, 0);
        return rtn;
    }

    JNIEXPORT jstring JNICALL Java_com_predictor_vijoz_jni_VIJOZJNI_getKey (
            JNIEnv * env, jobject, jobject contextObject){
        char *sha1 = getSha1(env, contextObject);
        jstring str = char2Jstring(env, sha1);
        return str;
    }

    JNIEXPORT jboolean JNICALL Java_com_predictor_vijoz_jni_VIJOZJNI_getPermission (
            JNIEnv * env, jobject){
        if (canUse) {
            return true;
        } else {
            return false;
        }
    }


    JNIEXPORT jstring JNICALL Java_com_predictor_vijoz_jni_VIJOZJNI_getStartCode (
                JNIEnv * env, jobject){
         jstring str = char2Jstring(env, app_key);
         return str;
    }


 JNIEXPORT void JNICALL Java_com_predictor_vijoz_jni_VIJOZJNI_exit (
                JNIEnv * env, jobject){
          std::this_thread::sleep_for(std::chrono::seconds(3));//3秒后执行下面代码
          exitApplication(env,0);//退出app
    }

    JNIEXPORT jboolean JNICALL Java_com_predictor_vijoz_jni_VIJOZJNI_getToken (
            JNIEnv * env,
            jobject,
            jobject contextObject,
            jstring key,
            jint date,
            jint type,
            jint nowtime){

        char *sha1 = getSha1(env, contextObject);
        char *key1 = jstringToChar(env, key);

       if (type == 0) {
           jboolean result = checkValidity(env, sha1);
           jboolean keyR = checkKey(env, key1);

           if (result && keyR) {
           jboolean judgment = JudgmentTime(env,contextObject,date);//时间是否过期
           if(nowtime==-1){
            if (judgment) {
                canUse = true;
                return true;
            } else {
                canUse = false;
                return false;
            }
           }else{
            jboolean netTimeJudgment = JudgmentTime(env,nowtime,date);//比对和网络时间
               if (netTimeJudgment) {
                  canUse = true;
                  return true;
              } else {
                   canUse = false;
                   return false;
              }
           }
           } else {
                canUse = false;
                return false;
           }
       }else if(type == 1) {
           jboolean result = checkValidity(env, sha1);
           jboolean keyR = checkKey(env, key1);
           if (result && keyR) {
                canUse = true;
                return true;
           } else {
                canUse = false;
                return false;
           }
       }else if(type == 2) {
            jboolean judgment = JudgmentTime(env,contextObject,date);//时间是否过期
                     if(nowtime==-1){
                      if (judgment) {
                          canUse = true;
                          return true;
                      } else {
                          canUse = false;
                          return false;
                      }
                     }else{
                      jboolean netTimeJudgment = JudgmentTime(env,nowtime,date);//比对和网络时间
                         if (netTimeJudgment) {
                            canUse = true;
                            return true;
                        } else {
                             canUse = false;
                             return false;
                        }
                     }
       }else if(type == 3) {
             canUse = true;
             return true;
       }else{
            exitApplication(env,0);
            canUse = false;
            return false;
       }

        /**
         if(result){
         return env->NewStringUTF("获取Token成功");
         }else{
         return env->NewStringUTF("获取失败，请检查valid.cpp文件配置的sha1值");
         }
         **/
    }



#ifdef __cplusplus
}
#endif
#endif