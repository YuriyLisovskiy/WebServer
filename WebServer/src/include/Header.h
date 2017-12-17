#pragma once
#pragma comment (lib, "Ws2_32.lib")

#define SERVER_IP "0.0.0.0"		// server ip
#define START_PORT 8000			// port for starting the server
#define MAX_SERVERD 1000		// max number of clinets per one port.

#if defined(_WIN32) || defined(_WIN64)
#include <direct.h>
#define BASE_DIR std::string(_getcwd(0, 0)) + "/"
#elif defined(__unix) || defined(__unix__) || defined(__APPLE__) || defined(__MACH__) || defined(__linux__) || defined(__FreeBSD__)
#include <unistd.h>
#define BASE_DIR std::string(getcwd(0, 0)) + "/"
#endif

enum rMethod
{
	Get,
	Post,
	Put,
	Delete,
	None
};

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
#define PRINT_SERVER_DATA(stream, IP, PORT)						\
{																\
	DATE_TIME_NOW(stream);										\
	(stream) << "\nWeb server version 1.0\n"					\
	"Started server at http://" << IP << ":" << PORT << "/\n"	\
	"Quit the server with CTRL - BREAK.\n";						\
}