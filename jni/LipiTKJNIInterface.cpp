#include <jni.h>
#include "LTKInkFileReader.h"
#include "LTKLipiEngineInterface.h"
#include "LipiEngineModule.h"
#include "lipiengine.h"
#include "LTKMacros.h"
#include "LTKInc.h"
#include "LTKTypes.h"
#include "LTKTrace.h"
#include <string>

// Allow logging support
#include <android/log.h>

#define LOGD(LOG_TAG, ...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGV(LOG_TAG, ...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define LOGE(LOG_TAG, ...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// Define some tags
#define LOG_JNI "JNI"

// Make random JNIEXPORT errors go away in eclipse
#ifndef JNIEXPORT
#define JNIEXPORT
#endif
#ifndef JNICALL
#define JNICALL
#endif

// Capture device settings
float gXDpi = 265.0f;
float gYDpi = 265.0f;
float gLatency = 0.0f; // ?? Look up what this is
float gSamplingRate = 1.0f; // ?? Look up what this is
bool gUseUniformSamplingRate = true;

// LipiTk engine vars
LTKLipiEngineInterface* gEngine;
LTKShapeRecognizer* gShapeReco = NULL;

extern "C" {
	JNIEXPORT void JNICALL Java_LipiTKJNIInterface_initializeNative(JNIEnv *env, jobject this_object, jstring lipiDirectory, jstring project, jstring recognizer);
	JNIEXPORT jobjectArray JNICALL Java_LipiTKJNIInterface_recognizeNative(JNIEnv *env, jobject this_object, jobject strokeobj);
}

JNIEXPORT void JNICALL Java_LipiTKJNIInterface_initializeNative(JNIEnv *env, jobject this_object, jstring lipiDirectory, jstring project, jstring recognizer) {
	int result;
	char* lipitkLocation = (char*)env->GetStringUTFChars(lipiDirectory, NULL); // ?? Need to release this

	// Load the engine
	LTKLipiEngineInterface* gEngine = createLTKLipiEngine();
	gEngine->setLipiRootPath(lipitkLocation);

	// Initialize the engine
	result = gEngine->initializeLipiEngine();
	if(result != SUCCESS) {
		LOGE(LOG_JNI, "Error initializing LipiTk Engine");
		return; // Possibly should delete some stuff before returning
	}

	// Load the shape recognizer
	std::string projectStr = std::string((char*)env->GetStringUTFChars(project, NULL));
	gEngine->createShapeRecognizer(projectStr, &gShapeReco);
	if(gShapeReco == NULL) {
		LOGE(LOG_JNI, "Error creating a shape recognizer");
		return; // Possibly should delete some stuff before returning
	}

	// Load the model data for the shape recognizer
	result = gShapeReco->loadModelData();
	if(result != SUCCESS) {
		LOGE(LOG_JNI, "Error loading model data");
		return; // Possibly should delete some stuff before returning
	}
}

JNIEXPORT jobjectArray JNICALL Java_LipiTKJNIInterface_recognizeNative(JNIEnv *env, jobject this_object, jobject strokeobj) {

}
