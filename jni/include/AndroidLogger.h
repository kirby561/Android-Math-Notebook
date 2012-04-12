#ifndef ANDROIDLOGGER_INCLUDED
#define ANDROIDLOGGER_INCLUDED

// Allow logging support
#include <android/log.h>
#include <string>
#include <ostream>
#include <sstream>


#define LOGD(LOG_TAG, ...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGV(LOG_TAG, ...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define LOGE(LOG_TAG, ...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#define LOG_ANDROIDLOGGER_TAG "AndroidLogger"

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

	AndroidLogger& operator<< (const char* val) {
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
		LOGV(LOG_ANDROIDLOGGER_TAG, "%s", buffer.c_str());
		buffer = "";
	}

};

#endif
