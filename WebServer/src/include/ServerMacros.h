#pragma once
#include "CommonMacros.h"

#define SERVER_IP "127.0.0.1"	// server ip (localhost)
#define START_PORT 8000			// port for starting the server
#define MAX_SERVERD 1000		// max number of clinets per port.

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

#if defined(_WIN32) || defined(_WIN64)
#include <winsock2.h>
#define SOCK SOCKET
#include <ws2tcpip.h>
#include <direct.h>
#pragma comment (lib, "Ws2_32.lib")
#define BASE_DIR std::string(_getcwd(0, 0)) + "/"
#define INVAL_SOCK INVALID_SOCKET
#define WSA_CLEANUP WSACleanup()
#define SOCK_ERROR SOCKET_ERROR
#define SOCK_SEND SD_SEND
#define SOCK_RECEIVE SD_RECEIVE
#elif defined(__unix) || defined(__unix__) || defined(__APPLE__) || defined(__MACH__) || defined(__linux__) || defined(__FreeBSD__)
#include <sys/types.h> 
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string>
#define SOCK int
std::string getRootDir();
#define BASE_DIR getRootDir()
#define INVALID_SOCK (-1)
#define CLOSE_SOCK(sock) close(sock)
#define WSA_CLEANUP /**/
#define SOCK_ERROR (-1)
#define SOCK_SEND SHUT_RDWR
#define SOCK_RECEIVE SHUT_RDWR
#endif
