#pragma once
#pragma comment (lib, "Ws2_32.lib")

#define SERVER_IP "0.0.0.0"		// server id (default is localhost)
#define START_PORT 8000			// port for starting the server
#define MAX_SERVERD 100			// max number of clinets per one port.

#if defined(_WIN32) || defined(_WIN64)
#include <direct.h>
#define BASE_DIR std::string(_getcwd(0, 0)) + "/"
#elif defined(__unix) || defined(__unix__) || defined(__APPLE__) || defined(__MACH__) || defined(__linux__) || defined(__FreeBSD__)
#include <unistd.h>
#define BASE_DIR std::string(getcwd(0, 0)) + "/"
#endif
