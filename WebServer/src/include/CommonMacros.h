#pragma once

#include <ctime>
#if defined(_WIN32) || defined(_WIN64)
#define DATE_TIME_NOW(stream)									\
{																\
	time_t now = time(nullptr);									\
	struct tm tstruct;											\
	char buf[80];												\
	localtime_s(&tstruct, &now);								\
	strftime(buf, sizeof(buf), "[%d/%b/%Y %r]", &tstruct);		\
	(stream) << buf;											\
}
#elif defined(__unix) || defined(__unix__) || defined(__APPLE__) || defined(__MACH__) || defined(__linux__) || defined(__FreeBSD__)
#define DATE_TIME_NOW(stream)									\
{																\
	time_t now = time(nullptr);									\
	struct tm tstruct;											\
	char buf[80];												\
	localtime_r(&now, &tstruct);								\
	strftime(buf, sizeof(buf), "[%d/%b/%Y %r]", &tstruct);		\
	(stream) << buf;											\
}
#endif
