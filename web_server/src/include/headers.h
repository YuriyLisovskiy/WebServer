#pragma once

#ifndef WEB_SERVER_HEADERS_H
#define WEB_SERVER_HEADERS_H

#define SERVER_IP "0.0.0.0"		    // server ip
#define SERVER_PORT "8000"		    // port for starting the server

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
#define LOCAL_TIME(timeNow, structure) localtime_s(&(structure), &(timeNow));
#define MSG_SIZE int

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
#define INVALID_SOCK (SOCK)(~0)
#define CLOSE_SOCK(sock) close(sock)
#define WSA_CLEANUP /**/
#define SOCK_ERROR (-1)
#define SOCK_SEND SHUT_RDWR
#define SOCK_RECEIVE SHUT_RDWR
#define WSA_LAST_ERR '\n'
#define WSA_STARTUP /**/
#define LOCAL_TIME(timeNow, structure) localtime_r(&(timeNow), &(structure))
#define MSG_SIZE ssize_t

#endif

#include <ctime>

#define DATE_TIME_NOW(stream, format)							\
{																\
	time_t now = time(nullptr);									\
	struct tm tstruct = {}; 									\
	char buf[80];												\
	LOCAL_TIME(now, tstruct);								    \
	strftime(buf, sizeof(buf), (format), &tstruct);	        	\
	(stream) << buf;											\
}

#define PRINT_SERVER_DATA(stream, IP, PORT)						    \
{																    \
	(stream) << '[';												\
	DATE_TIME_NOW(stream, "%d/%b/%Y %r");					    	\
	(stream) << ']';												\
	(stream) << "\nWeb server version 1.0\n"					    \
	"Started server at http://" << (IP) << ":" << (PORT) << "/\n"	\
	"Quit the server with CTRL - BREAK.\n";						    \
}

#define __HTTP_BEGIN namespace http {
#define __HTTP_END }

enum REQUEST_METHOD
{
	GET,
	POST,
	PUT,
	DElETE,
	HEAD,
	CONNECT,
	OPTIONS,
	TRACE,
	PATCH,
	REQUEST_NONE
};

enum CONTENT_TYPE
{
	JSON,
	X_WWW_FORM_URLENCODED,
	CONTENT_NONE
};

#define JSON_TYPE "application/json"
#define X_WWW_FORM_URLENCODED_TYPE "application/x-www-form-urlencoded"
#define IMAGE_TYPE(type) (type) == "png" || (type) == "jpg" || (type) == "jpeg" || (type) == "ico" || (type) == "gif"

#endif
