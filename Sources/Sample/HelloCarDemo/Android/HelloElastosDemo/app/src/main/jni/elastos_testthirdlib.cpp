
#include <jni.h>

//For Elastos
#include "Elastos.HelloCarDemo.Export.h"

#define JNIREG_CLASS "org/elastos/runtime/sample/helloelastos/TestUtils"

static jstring JNICALL nativeGetTestString(JNIEnv *env, jobject jobj)
{
    return env->NewStringUTF("nativeGetTestString");
}

static jstring JNICALL nativeGetHelloCarString(JNIEnv *env, jobject jobj)
{
    AutoPtr<IHelloCar> helloCarObj;
    ECode ec = CHelloCar::New((IHelloCar**)&helloCarObj);
    if (ec != NOERROR || helloCarObj == NULL) {
        return 0;
    }
    String helloString;

    helloCarObj->Hello(&helloString);

    String newString("[native ccccc] Hello, ");
    newString += helloString + "!";

    return env->NewStringUTF(newString.string());
}

static const JNINativeMethod gMethods[] = {
    {"nativeGetTestString", "()Ljava/lang/String;", (void*)nativeGetTestString},
    {"nativeGetHelloCarString", "()Ljava/lang/String;", (void*)nativeGetHelloCarString}
};

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved){
    JNIEnv * env;
    jclass cls;
    jint result = -1;
    if(vm->GetEnv((void **)&env,JNI_VERSION_1_6) != JNI_OK){
        return JNI_ERR;
    }

    cls = env->FindClass(JNIREG_CLASS);
    env->RegisterNatives(cls, gMethods, sizeof(gMethods)/sizeof(JNINativeMethod));
    return JNI_VERSION_1_6;
}
