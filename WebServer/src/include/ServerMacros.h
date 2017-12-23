#pragma once

#ifndef WEB_SERVER_SERVER_MACROS_H
#define WEB_SERVER_SERVER_MACROS_H

#include "CommonMacros.h"

#define SERVER_IP "127.0.0.1"	// server ip (localhost)
#define START_PORT 8000			// port for starting the server
#define MAX_SERVED 1000		    // max number of clinets per port.

#define PRINT_SERVER_DATA(stream, IP, PORT)						    \
{																    \
	(stream) << '[';												\
	DATE_TIME_NOW(stream);									    	\
	(stream) << ']';												\
	(stream) << "\nWeb server version 1.0\n"					    \
	"Started server at http://" << (IP) << ":" << (PORT) << "/\n"	\
	"Quit the server with CTRL - BREAK.\n";						    \
}

#define MAX_BUFF_SIZE 1024

#if defined(_WIN32) || defined(_WIN64)

#include <winsock2.h>
#include <ws2tcpip.h>
#include <direct.h>

#pragma comment (lib, "Ws2_32.lib")

#define SOCK SOCKET
#define BASE_DIR std::string(_getcwd(0, 0)) + "/"
#define INVALID_SOCK INVALID_SOCKET
#define WSA_CLEANUP WSACleanup()
#define CLOSE_SOCK(sock) closesocket(sock)
#define WSA_LAST_ERR " failed with error #" << WSAGetLastError() << '\n'
#define SOCK_ERROR SOCKET_ERROR
#define SOCK_SEND SD_SEND
#define SOCK_RECEIVE SD_RECEIVE
#define WSA_STARTUP									\
{													\
	int status;										\
	WSADATA wsaData;								\
	status = WSAStartup(MAKEWORD(2, 2), &wsaData);	\
	if (status != 0)								\
	{												\
		std::cerr << "SERVER ERROR: 'HttpServer::run()': 'WSAStartup(MAKEWORD(2, 2), &wsaData)' failed with error #" << status << '\n';\
		return;										\
	}												\
}

#elif defined(__unix) || defined(__unix__) || defined(__APPLE__) || defined(__MACH__) || defined(__linux__) || defined(__FreeBSD__)

#include <sys/types.h> 
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string>

std::string getRootDir();

#define SOCK int
#define BASE_DIR getRootDir()
#define INVALID_SOCK (SOCK)(~0)//(-1)
#define CLOSE_SOCK(sock) close(sock)
#define WSA_CLEANUP /**/
#define SOCK_ERROR (-1)
#define SOCK_SEND SHUT_RDWR
#define SOCK_RECEIVE SHUT_RDWR
#define WSA_LAST_ERR '\n'
#define WSA_STARTUP /**/

#endif

#endif
