#include <jni.h>

// Make random JNIEXPORT errors go away in eclipse
#ifndef JNIEXPORT
#define JNIEXPORT
#endif
#ifndef JNICALL
#define JNICALL
#endif

extern "C" {
	JNIEXPORT void JNICALL Java_LipiTKJNIInterface_initializeNative(JNIEnv *env, jobject this_object, jstring lipiDirectory, jstring project, jstring recognizer);
	JNIEXPORT jobjectArray JNICALL Java_LipiTKJNIInterface_recognizeNative(JNIEnv *env, jobject this_object, jobject strokeobj);
}


JNIEXPORT void JNICALL Java_LipiTKJNIInterface_initializeNative(JNIEnv *env, jobject this_object, jstring lipiDirectory, jstring project, jstring recognizer) {

}

JNIEXPORT jobjectArray JNICALL Java_LipiTKJNIInterface_recognizeNative(JNIEnv *env, jobject this_object, jobject strokeobj) {

}
