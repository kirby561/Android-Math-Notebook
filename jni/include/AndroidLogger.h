#ifndef ANDROIDLOGGER_INCLUDED
#define ANDROIDLOGGER_INCLUDED

// Allow logging support
#include <android/log.h>
#include <string>
#include <ostream>
#include <sstream>

#define LOG_ANDROIDLOGGER_TAG "AndroidLogger"

#define  LOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE,LOG_ANDROIDLOGGER_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_ANDROIDLOGGER_TAG,__VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_ANDROIDLOGGER_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_ANDROIDLOGGER_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_ANDROIDLOGGER_TAG,__VA_ARGS__)
#define  LOGTV(TAG, ...)  __android_log_print(ANDROID_LOG_VERBOSE,STRVALUE(APPEND_PIDROID_NATIVE(TAG)),__VA_ARGS__)
#define  LOGTD(TAG, ...)  __android_log_print(ANDROID_LOG_DEBUG,STRVALUE(APPEND_PIDROID_NATIVE(TAG)),__VA_ARGS__)
#define  LOGTI(TAG, ...)  __android_log_print(ANDROID_LOG_INFO,APPEND_PIDROID_NATIVE(TAG),__VA_ARGS__)
#define  LOGTW(TAG, ...)  __android_log_print(ANDROID_LOG_WARN,STRVALUE(APPEND_PIDROID_NATIVE(TAG)),__VA_ARGS__)
#define  LOGTE(TAG, ...)  __android_log_print(ANDROID_LOG_ERROR,STRVALUE(APPEND_PIDROID_NATIVE(TAG)),__VA_ARGS__)

class AndroidLogger
{
	std::string buffer;
public:
	AndroidLogger() {
	}

	AndroidLogger& operator<< (int val) {
		std::ostringstream oss;
		oss << val;
		buffer += oss.str();
		return *this;
	}

	AndroidLogger& operator<< (const std::string& val) {
		std::ostringstream oss;
		oss << val;
		buffer += oss.str();
		return *this;
	}

	AndroidLogger& operator<< (unsigned int val) {
		std::ostringstream oss;
		oss << val;
		buffer += oss.str();
		return *this;
	}


	AndroidLogger& operator<< (float val) {
		std::ostringstream oss;
		oss << val;
		buffer += oss.str();
		return *this;
	}

	AndroidLogger& operator<< (void* val) {
		//WHAT DO?
		buffer+="Trying to print a void*";
		LOGV(LOG_ANDROIDLOGGER_TAG, "%s", buffer.c_str());
		buffer = "";
		return *this;
	}

	AndroidLogger& operator<< (const char* val) {
		std::ostringstream oss;
		oss << val;
		buffer += oss.str();
		return *this;
	}

	AndroidLogger& operator<< (double val) {
		std::ostringstream oss;
		oss << val;
		buffer += oss.str();
		return *this;
	}

	AndroidLogger& operator<< (std::ostream &f(std::ostream &)) {
		// Do nothing
		std::ostringstream oss;
		oss << "\n";
		buffer += oss.str();

		LOGV(LOG_ANDROIDLOGGER_TAG, "%s", buffer.c_str());
		buffer = "";
		return *this;
	}

	void flush() {
		//LOGV(LOG_ANDROIDLOGGER_TAG, "%s", buffer.c_str());
		//buffer = "";
	}

};

#endif
