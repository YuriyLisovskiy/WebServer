#pragma once
#include "CommonMacros.h"

#define SERVER_IP "127.0.0.1"	// server ip (localhost)
#define START_PORT 8000			// port for starting the server
#define MAX_SERVERD 1000		// max number of clinets per port.

#include <direct.h>
#pragma comment (lib, "Ws2_32.lib")
#define BASE_DIR std::string(_getcwd(0, 0)) + "/"

#define PRINT_SERVER_DATA(stream, IP, PORT)						\
{																\
	DATE_TIME_NOW(stream);										\
	(stream) << "\nWeb server version 1.0\n"					\
	"Started server at http://" << IP << ":" << PORT << "/\n"	\
	"Quit the server with CTRL - BREAK.\n";						\
}

#define __HTTP_BEGIN namespace http {
#define __HTTP_END }

#define MAX_BUFF_SIZE 1024
