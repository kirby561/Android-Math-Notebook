#ifndef ANDROIDLOGGER_INCLUDED
#define ANDROIDLOGGER_INCLUDED

#include <ostream>
#include <sstream>
#include <iostream>

// Allow logging support
#include <android/log.h>

#define LOGD(LOG_TAG, ...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGV(LOG_TAG, ...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define LOGE(LOG_TAG, ...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#define LOG_ANDROIDLOGGER_TAG "AndroidLogger"

class AndroidLogger: public std::basic_ostream<char, std::char_traits<char> >
{
public:
	AndroidLogger() {
	}

	virtual std::basic_ostream<char, std::char_traits<char> >& operator<< (bool& val ) {
		if (val)
			LOGV(LOG_ANDROIDLOGGER_TAG, "true");
		else
			LOGV(LOG_ANDROIDLOGGER_TAG, "false");
		return *this;
	}

	virtual std::basic_ostream<char, std::char_traits<char> >& operator<< (short& val ) {
		LOGV(LOG_ANDROIDLOGGER_TAG, "%hd", val);
		return *this;
	}

	virtual std::basic_ostream<char, std::char_traits<char> >& operator<< (unsigned short& val ) {
		LOGV(LOG_ANDROIDLOGGER_TAG, "%hu", val);
		return *this;
	}

	virtual std::basic_ostream<char, std::char_traits<char> >& operator<< (int& val ) {
		LOGV(LOG_ANDROIDLOGGER_TAG, "%d", val);
		return *this;
	}

	virtual std::basic_ostream<char, std::char_traits<char> >& operator<< (unsigned int& val ) {
		LOGV(LOG_ANDROIDLOGGER_TAG, "%u", val);
		return *this;
	}

	virtual std::basic_ostream<char, std::char_traits<char> >& operator<< (long& val ) {
		LOGV(LOG_ANDROIDLOGGER_TAG, "%ld", val);
		return *this;
	}

	virtual std::basic_ostream<char, std::char_traits<char> >& operator<< (unsigned long& val ) {
		LOGV(LOG_ANDROIDLOGGER_TAG, "%lu", val);
		return *this;
	}

	virtual std::basic_ostream<char, std::char_traits<char> >& operator<< (float& val ) {
		LOGV(LOG_ANDROIDLOGGER_TAG, "%f", val);
		return *this;
	}

	virtual std::basic_ostream<char, std::char_traits<char> >& operator<< (double& val ) {
		LOGV(LOG_ANDROIDLOGGER_TAG, "%f", val);
		return *this;
	}

	virtual std::basic_ostream<char, std::char_traits<char> >& operator<< (long double& val ) {
		LOGV(LOG_ANDROIDLOGGER_TAG, "%lf", val);
		return *this;
	}

	virtual std::basic_ostream<char, std::char_traits<char> >& operator<< (void*& val ) {
		LOGV(LOG_ANDROIDLOGGER_TAG, "void*, address=%x", val);
		return *this;
	}

	virtual std::basic_ostream<char, std::char_traits<char> >& operator<< (basic_streambuf<char,std::char_traits<char> >* sb ){
		std::ostringstream ss;
		ss << std::cin.rdbuf();
		std::string s = ss.str();

		LOGV(LOG_ANDROIDLOGGER_TAG, "%s", s.c_str());
		return *this;
	}

};

#endif
