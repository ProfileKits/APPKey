#include <stdio.h>
#include <stdlib.h>
#include <jni.h>
#include <android/log.h>

//签名信息 6EC731ADA41FDE976DCDBEC8A17B7DE93A30E2EC
//我的电脑
//const char *app_sha_debug ="E72670395B49E12C4F811CBC5E18A0F13A2AC658";
const char *app_sha_release ="A1F30666F1B10FFFA169EE875A1CF95F2FA62A58";
//我的电脑sha E72670395B49E12C4F811CBC5E18A0F13A2AC658

//app_key之后要通过一个算法验算这个key是否正确
const char *app_key="krzd8Uj5p8rviZ1Xjxm7YcrBW2vYRs2cRbspWiQY2IM=";
const char hexcode[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
//
//const int exp_time = 1981056000;//2023-4-10 Entry app过期时间 改成下次项目交付时间戳（秒） //1681056000
//const char *exp_time = "2223:03:01 00:00:00";//2023-3-1 Entry app过期时间 改成下次项目交付时间戳（秒） //20000000000

char* getSha1(JNIEnv *env, jobject context_object){
    //上下文对象
    jclass context_class = env->GetObjectClass(context_object);

    //反射获取PackageManager
    jmethodID methodId = env->GetMethodID(context_class, "getPackageManager", "()Landroid/content/pm/PackageManager;");
    jobject package_manager = env->CallObjectMethod(context_object, methodId);
    if (package_manager == NULL) {
        return NULL;
    }

    //反射获取包名
    methodId = env->GetMethodID(context_class, "getPackageName", "()Ljava/lang/String;");
    jstring package_name = (jstring)env->CallObjectMethod(context_object, methodId);
    if (package_name == NULL) {
        return NULL;
    }
    env->DeleteLocalRef(context_class);

    //获取PackageInfo对象
    jclass pack_manager_class = env->GetObjectClass(package_manager);
    methodId = env->GetMethodID(pack_manager_class, "getPackageInfo", "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    env->DeleteLocalRef(pack_manager_class);
    jobject package_info = env->CallObjectMethod(package_manager, methodId, package_name, 0x40);
    if (package_info == NULL) {
        return NULL;
    }
    env->DeleteLocalRef(package_manager);

    //获取签名信息
    jclass package_info_class = env->GetObjectClass(package_info);
    jfieldID fieldId = env->GetFieldID(package_info_class, "signatures", "[Landroid/content/pm/Signature;");
    env->DeleteLocalRef(package_info_class);
    jobjectArray signature_object_array = (jobjectArray)env->GetObjectField(package_info, fieldId);
    if (signature_object_array == NULL) {
        return NULL;
    }
    jobject signature_object = env->GetObjectArrayElement(signature_object_array, 0);
    env->DeleteLocalRef(package_info);

    //签名信息转换成sha1值
    jclass signature_class = env->GetObjectClass(signature_object);
    methodId = env->GetMethodID(signature_class, "toByteArray", "()[B");
    env->DeleteLocalRef(signature_class);
    jbyteArray signature_byte = (jbyteArray) env->CallObjectMethod(signature_object, methodId);
    jclass byte_array_input_class=env->FindClass("java/io/ByteArrayInputStream");
    methodId=env->GetMethodID(byte_array_input_class,"<init>","([B)V");
    jobject byte_array_input=env->NewObject(byte_array_input_class,methodId,signature_byte);
    jclass certificate_factory_class=env->FindClass("java/security/cert/CertificateFactory");
    methodId=env->GetStaticMethodID(certificate_factory_class,"getInstance","(Ljava/lang/String;)Ljava/security/cert/CertificateFactory;");
    jstring x_509_jstring=env->NewStringUTF("X.509");
    jobject cert_factory=env->CallStaticObjectMethod(certificate_factory_class,methodId,x_509_jstring);
    methodId=env->GetMethodID(certificate_factory_class,"generateCertificate",("(Ljava/io/InputStream;)Ljava/security/cert/Certificate;"));
    jobject x509_cert=env->CallObjectMethod(cert_factory,methodId,byte_array_input);
    env->DeleteLocalRef(certificate_factory_class);
    jclass x509_cert_class=env->GetObjectClass(x509_cert);
    methodId=env->GetMethodID(x509_cert_class,"getEncoded","()[B");
    jbyteArray cert_byte=(jbyteArray)env->CallObjectMethod(x509_cert,methodId);
    env->DeleteLocalRef(x509_cert_class);
    jclass message_digest_class=env->FindClass("java/security/MessageDigest");
    methodId=env->GetStaticMethodID(message_digest_class,"getInstance","(Ljava/lang/String;)Ljava/security/MessageDigest;");
    jstring sha1_jstring=env->NewStringUTF("SHA1");
    jobject sha1_digest=env->CallStaticObjectMethod(message_digest_class,methodId,sha1_jstring);
    methodId=env->GetMethodID(message_digest_class,"digest","([B)[B");
    jbyteArray sha1_byte=(jbyteArray)env->CallObjectMethod(sha1_digest,methodId,cert_byte);
    env->DeleteLocalRef(message_digest_class);

    //转换成char
    jsize array_size=env->GetArrayLength(sha1_byte);
    jbyte* sha1 =env->GetByteArrayElements(sha1_byte,NULL);
    char *hex_sha=new char[array_size*2+1];
    for (int i = 0; i <array_size ; ++i) {
        hex_sha[2*i]=hexcode[((unsigned char)sha1[i])/16];
        hex_sha[2*i+1]=hexcode[((unsigned char)sha1[i])%16];
    }
    hex_sha[array_size*2]='\0';

    return hex_sha;
}


void exitApplication(JNIEnv *env, jint flag){
    jclass temp_clazz = NULL;
    jmethodID mid_static_method;
    // 1、从classpath路径下搜索ClassMethod这个类，并返回该类的Class对象
    temp_clazz = env->FindClass("java/lang/System");
    mid_static_method = env->GetStaticMethodID(temp_clazz,"exit","(I)V");
    env->CallStaticVoidMethod(temp_clazz,mid_static_method,flag);
    env->DeleteLocalRef(temp_clazz);
}

jboolean checkValidity(JNIEnv *env,char *sha1){
     if (strcmp(sha1,app_sha_release)==0)
        {
            return true;
        }else{
            exitApplication(env,0);
        }
    return false;
}




jboolean checkKey(JNIEnv *env,char *key){
    //比较key
    if (strcmp(key,app_key)==0)
    {
        return true;
    }else{
     exitApplication(env,0);
    }
    return false;
}


//转换时间
int standard_to_stamp(const char *str_time){//这个转换有点问题，暂时没有使用
            struct tm stm;
            int iY, iM, iD, iH, iMin, iS;

            memset(&stm,0,sizeof(stm));
            iY = atoi(str_time);
            iM = atoi(str_time+5);
            iD = atoi(str_time+8);
            iH = atoi(str_time+11);
            iMin = atoi(str_time+14);
            iS = atoi(str_time+17);

            stm.tm_year=iY-1900;
            stm.tm_mon=iM-1;
            stm.tm_mday=iD;
            stm.tm_hour=iH;
            stm.tm_min=iMin;
            stm.tm_sec=iS;
        //__android_log_print(ANDROID_LOG_VERBOSE, "JNI打印:","时间: %d-%d-%d %d:%d:%d",iY,iM,iD,iH,iMin,iS);//过期时间
         /*printf("%d-%0d-%0d %0d:%0d:%0d\n", iY, iM, iD, iH, iMin, iS);*/
        //标准时间格式例如：2016:08:02 12:12:30
        return (int)mktime(&stm);
}




jboolean JudgmentTime(JNIEnv *env, jobject instance,int exit_time) {//带时间限制
    //int t = standard_to_stamp(exp_time);
    int t = exit_time;//有效期时间(转换成现在的时间)
    time_t now;//当前系统的时间
    int unixTime = (int)time(&now);//当前系统的时间
    //__android_log_print(ANDROID_LOG_VERBOSE, "JNI打印:","当前时间: %d",unixTime);//当前系统的时间
    //__android_log_print(ANDROID_LOG_VERBOSE, "JNI打印:","时间: %d",t);//过期时间
    if (t > unixTime){//过期时间大于当前时间
  //__android_log_print(ANDROID_LOG_VERBOSE, "JNI打印:","now值: %d",1);
    return true;
    }else{
     exitApplication(env,0);
  //__android_log_print(ANDROID_LOG_VERBOSE, "JNI打印:","now值: %d",2);
    return false;
    }
}

jboolean JudgmentTime(JNIEnv *env, int nowtime,int exit_time) {//带时间限制
    //int t = standard_to_stamp(exp_time);
    int t = exit_time;//有效期时间(转换成现在的时间)
    int unixTime = nowtime;//当前网络的时间
    //__android_log_print(ANDROID_LOG_VERBOSE, "JNI打印:","当前时间: %d",unixTime);//当前系统的时间
    //__android_log_print(ANDROID_LOG_VERBOSE, "JNI打印:","时间: %d",t);//过期时间
    if (t > unixTime){//过期时间大于当前时间
  //__android_log_print(ANDROID_LOG_VERBOSE, "JNI打印:","now值: %d",1);
    return true;
    }else{
     exitApplication(env,0);
  //__android_log_print(ANDROID_LOG_VERBOSE, "JNI打印:","now值: %d",2);
    return false;
    }
}


jboolean JudgmentTime(JNIEnv *env, jobject instance) {//解除时间限制
    return true;
}