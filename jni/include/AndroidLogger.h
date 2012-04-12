#ifndef ANDROIDLOGGER_INCLUDED
#define ANDROIDLOGGER_INCLUDED

// Allow logging support
#include <android/log.h>
#include <string>
#include <ostream>


#define LOGD(LOG_TAG, ...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGV(LOG_TAG, ...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define LOGE(LOG_TAG, ...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#define LOG_ANDROIDLOGGER_TAG "AndroidLogger"

class AndroidLogger
{
public:
	AndroidLogger() {
	}

	AndroidLogger& operator<< (int val) {
		LOGV(LOG_ANDROIDLOGGER_TAG, "%d", val);
		return *this;
	}

	AndroidLogger& operator<< (const std::string& val) {
		LOGV(LOG_ANDROIDLOGGER_TAG, "%s", val.c_str());
		return *this;
	}

	AndroidLogger& operator<< (unsigned int val) {
		LOGV(LOG_ANDROIDLOGGER_TAG, "%u", val);
		return *this;
	}

	AndroidLogger& operator<< (const char* val) {
		LOGV(LOG_ANDROIDLOGGER_TAG, "%s", val);
		return *this;
	}

	AndroidLogger& operator<< (std::ostream &f(std::ostream &)) {
		// Do nothing
		return *this;
	}

	void flush() {
		// Do nothing
	}

};

#endif
