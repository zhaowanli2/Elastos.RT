
#include <jni.h>
#include <string>
#include "Key.h"
#include "Utils.h"

using namespace Elastos::ElaWallet;

static jstring JNICALL nativePublicKeyToIdAddress(JNIEnv *env, jobject clazz, jstring jpublicKey)
{
    const char* publicKey = env->GetStringUTFChars(jpublicKey, NULL);

    CMBlock pubKey = Utils::decodeHex(publicKey);
    BRKey key;
    memcpy(key.pubKey, pubKey, pubKey.GetSize());
    key.compressed = (pubKey.GetSize() <= 33);
    std::string id = Key::publicKeyToIdAddress(&key);

    env->ReleaseStringUTFChars(jpublicKey, publicKey);
    return env->NewStringUTF(id.c_str());
}

static jboolean JNICALL nativeVerifyByPublicKey(JNIEnv *env, jobject clazz, jstring jpublicKey, jstring jmessage, jstring jsignature)
{
    const char* publicKey = env->GetStringUTFChars(jpublicKey, NULL);
    const char* message = env->GetStringUTFChars(jmessage, NULL);
    const char* signature = env->GetStringUTFChars(jsignature, NULL);

    bool status = Key::verifyByPublicKey(publicKey, message, signature);

    env->ReleaseStringUTFChars(jpublicKey, publicKey);
    env->ReleaseStringUTFChars(jmessage, message);
    env->ReleaseStringUTFChars(jsignature, signature);
    return (jboolean)status;
}

#ifndef NELEM
# define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))
#endif

int jniRegisterNativeMethods(JNIEnv* env, const char* className, const JNINativeMethod* gMethods, int numMethods);

static const JNINativeMethod gMethods[] = {
    {"nativePublicKeyToIdAddress", "(Ljava/lang/String;)Ljava/lang/String;", (void*)nativePublicKeyToIdAddress},
    {"nativeVerifyByPublicKey", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z", (void*)nativeVerifyByPublicKey},
};

jint register_elastos_checkdid(JNIEnv *env)
{
    return jniRegisterNativeMethods(env, "org/elastos/checkdid/CheckDid",
        gMethods, NELEM(gMethods));
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved){
    JNIEnv * env;
    jclass cls;
    jint result = -1;
    if(vm->GetEnv((void **)&env,JNI_VERSION_1_6) != JNI_OK){
        return JNI_ERR;
    }

    register_elastos_checkdid(env);

    return JNI_VERSION_1_6;
}

int jniRegisterNativeMethods(JNIEnv* env, const char* className,
    const JNINativeMethod* gMethods, int numMethods)
{
    jclass cls = env->FindClass(className);
    env->RegisterNatives(cls, gMethods, numMethods);

    return 0;
}
