// Generated by uniffi-bindgen-react-native
#include <jni.h>
#include <jsi/jsi.h>
#include <ReactCommon/CallInvokerHolder.h>
#include "{{ self.config.project.cpp_filename() }}.h"
{%- let package_name = self.config.project.android.package_name.replace(".", "_") %}
{%- let name = self.config.project.name_upper_camel() %}
{%- let module_class_name = name|fmt("{}Module") %}
{%- let prefix = "Java_{}_{}"|format(package_name, module_class_name) %}
{%- let ns = self.config.project.cpp_namespace() %}

namespace jsi = facebook::jsi;
namespace react = facebook::react;

// TODO Remove `multiply` after seeing this work on iOS and Android.
extern "C"
JNIEXPORT jdouble JNICALL
{{ prefix }}_nativeMultiply(JNIEnv *env, jclass type, jdouble a, jdouble b) {
    return {{ ns }}::multiply(a, b);
}

// Installer coming from {{ module_class_name }}
extern "C"
JNIEXPORT jboolean JNICALL
{{ prefix }}_nativeInstallRustCrate(
    JNIEnv *env,
    jclass type,
    jlong rtPtr,
    jobject callInvokerHolderJavaObj
) {
    // https://github.com/realm/realm-js/blob/main/packages/realm/binding/android/src/main/cpp/io_realm_react_RealmReactModule.cpp#L122-L145
    // React Native uses the fbjni library for handling JNI, which has the concept of "hybrid objects",
    // which are Java objects containing a pointer to a C++ object. The CallInvokerHolder, which has the
    // invokeAsync method we want access to, is one such hybrid object.
    // Rather than reworking our code to use fbjni throughout, this code unpacks the C++ object from the Java
    // object `callInvokerHolderJavaObj` manually, based on reverse engineering the fbjni code.

    // 1. Get the Java object referred to by the mHybridData field of the Java holder object
    auto callInvokerHolderClass = env->GetObjectClass(callInvokerHolderJavaObj);
    auto hybridDataField = env->GetFieldID(callInvokerHolderClass, "mHybridData", "Lcom/facebook/jni/HybridData;");
    auto hybridDataObj = env->GetObjectField(callInvokerHolderJavaObj, hybridDataField);

    // 2. Get the destructor Java object referred to by the mDestructor field from the myHybridData Java object
    auto hybridDataClass = env->FindClass("com/facebook/jni/HybridData");
    auto destructorField =
        env->GetFieldID(hybridDataClass, "mDestructor", "Lcom/facebook/jni/HybridData$Destructor;");
    auto destructorObj = env->GetObjectField(hybridDataObj, destructorField);

    // 3. Get the mNativePointer field from the mDestructor Java object
    auto destructorClass = env->FindClass("com/facebook/jni/HybridData$Destructor");
    auto nativePointerField = env->GetFieldID(destructorClass, "mNativePointer", "J");
    auto nativePointerValue = env->GetLongField(destructorObj, nativePointerField);

    // 4. Cast the mNativePointer back to its C++ type
    auto nativePointer = reinterpret_cast<facebook::react::CallInvokerHolder*>(nativePointerValue);
    auto jsCallInvoker = nativePointer->getCallInvoker();

    auto runtime = reinterpret_cast<jsi::Runtime *>(rtPtr);
    return {{ ns }}::installRustCrate(*runtime, jsCallInvoker);
}

extern "C"
JNIEXPORT jboolean JNICALL
{{ prefix }}_nativeCleanupRustCrate(JNIEnv *env, jclass type, jlong rtPtr, jboolean a) {
    auto runtime = reinterpret_cast<jsi::Runtime *>(rtPtr);
    return {{ ns }}::cleanupRustCrate(*runtime, a);
}