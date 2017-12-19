#pragma once

#include <time.h>
#define DATE_TIME_NOW(stream)									\
{																\
	time_t now = time(nullptr);									\
	struct tm tstruct;											\
	char buf[80];												\
	localtime_s(&tstruct, &now);								\
	strftime(buf, sizeof(buf), "[%d/%b/%Y %r]", &tstruct);		\
	(stream) << buf;											\
}
