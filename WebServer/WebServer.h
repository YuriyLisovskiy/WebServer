//mainthreadsock define the entry point for console application
#pragma once
#define WIN32_LEAN_AND_MEAN

#include <ws2tcpip.h>
#include <winsock2.h>
#include <process.h>
#include <memory>
#include <thread>
#include <stdio.h>
#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <mutex>
#include <chrono>
#include <fstream>
#pragma comment (lib, "Ws2_32.lib")

static const char SERVER_IP[] = "127.0.0.1"; //default server ip (localhost)

void startThreadPorts(const int port, FILE *logFile);
void servClient(SOCKET clientInstance, int port, FILE *logfile);
void printBuffer(char* bufferPtr, int size);
void putClientOnDiferrentPort(SOCKET clientInstance, int port);
void getClientResource(SOCKET clientInstance);
void openFileWithPathAndSend(std::string filePath, SOCKET clientInstance);
void sendFile(FILE* m_file, SOCKET clientInstance);
std::string processRequest(char* bufferPtr);
std::string getFilePath(std::string p_toParse);
void printClientPortAndIP(SOCKET clientInstance, int port, FILE *logfile);
int roundRobinGetNextPort(int port);

std::mutex g_lockPrint;
std::mutex g_lockCounter;
std::recursive_mutex g_r_lock;
int g_counter = 1;
int g_clientId = 0;

void startThreadPorts(const int port, FILE *logFile)
{
	//create my listening socket and client socket which will be different client each time
	SOCKET listenSock;
	SOCKET client;
	int status;			//error handling
	sockaddr_in addr;	//structure to hold socket data
	socklen_t sa_size = sizeof(sockaddr_in);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	inet_pton(AF_INET, SERVER_IP, &(addr.sin_addr));
	listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSock == INVALID_SOCKET)
	{
		std::cerr << "Socket function failed with error: " << WSAGetLastError() << '\n';
		WSACleanup();
	}
	status = bind(listenSock, (sockaddr*)&addr, sizeof(sockaddr_in));
	if (status == SOCKET_ERROR)
	{
		std::cerr << "Bind function failed with error " << WSAGetLastError() << '\n';
		status = closesocket(listenSock);
		if (status == SOCKET_ERROR)
		{
			std::cerr << "Closesocket function failed with error " << WSAGetLastError() << '\n';
		}
		WSACleanup();
	}
	if (listen(listenSock, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cerr << "Listen function failed with error: " << WSAGetLastError() << '\n';
	}
//	std::cout << "Thread with id " << GetCurrentThreadId() << "is on port: " << port << '\n';
	int clientNum = 0;
	int breakAfterServed = 100; //this is the number of clients that will be served by the server
	bool listening = true;
	while (listening)
	{
		//if you want to change the number of clients than you have to change the number when server stop running
		if (clientNum == breakAfterServed / 5 && port != 2000)
		{
			std::chrono::milliseconds duration(2000);
			std::this_thread::sleep_for(duration);
			break;
		}
		if (clientNum == breakAfterServed && port == 2000)
		{
			std::chrono::milliseconds duration(2000);
			std::this_thread::sleep_for(duration);
			break;
		}
		if ((client = accept(listenSock, (sockaddr*)&addr, &sa_size)) != INVALID_SOCKET)
		{
			g_lockPrint.lock();
			g_lockPrint.unlock();
			std::thread t(servClient, client, port, logFile);
			t.detach(); //this will allow the thread run on its own
			clientNum++;
		}
		else
		{
			std::cerr << "Invalid client socket\n";
			std::cin.ignore();
		}
	}
}

void servClient(SOCKET clientInstance, int port, FILE *logfile)
{
	if (port == 2000)
	{
		putClientOnDiferrentPort(clientInstance, roundRobinGetNextPort(port));
	}
	else
	{
		//this is the timing part of the code
		std::chrono::time_point<std::chrono::system_clock> start, end;
		start = std::chrono::system_clock::now();
		printClientPortAndIP(clientInstance, port, logfile);

		getClientResource(clientInstance);	//this function will get the webpage for client or page not found

		end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end - start;
		std::time_t end_time = std::chrono::system_clock::to_time_t(end);
		wchar_t buf[26];
		errno_t err;
		err = _wctime_s(buf, 26, &end_time);
		if (err != 0)
		{
			std::cerr << "Invalid Arguments for _wctime_s. Error Code: " << err << '\n';
		}
		fwrite(buf, sizeof(wchar_t), 26, logfile);
		std::string requestTook = "Request took: " + std::to_string(elapsed_seconds.count()) + " seconds. \r\n";
		fwrite(requestTook.c_str(), sizeof(char), requestTook.size(), logfile);
	}
}

//this will guaratee that each port will get diferent number
int roundRobinGetNextPort(int port)
{
	std::lock_guard<std::mutex> lock(g_lockPrint);//this allow to change the wariable only in one thread
	g_clientId++;
	port = port + g_counter;
	g_counter++;
	if (g_counter == 6)
	{
		g_counter = 1;
	}
	return port;
}

void printClientPortAndIP(SOCKET clientInstance, int port, FILE *logfile)
{
	//this gets clients ip from sock_addr_in
	struct sockaddr_in addr;
	socklen_t addr_size = sizeof(struct sockaddr_in);
	int res = getsockname(clientInstance, (struct sockaddr *)&addr, &addr_size);
	sockaddr_in* pV4Addr = (struct sockaddr_in*)&addr;
	int ipAddr = pV4Addr->sin_addr.s_addr;
	char clientIp[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &ipAddr, clientIp, INET_ADDRSTRLEN);
	std::string cId = "ID: " + std::to_string(g_clientId) + " ";
	std::string portStr = "The Client port is: " + std::to_string(port) + " ";
	std::string clientIP = "The Client IP is: ";
	clientIP.append(clientIp);
	clientIP.append(" ");

	fwrite(cId.c_str(), sizeof(char), cId.size(), logfile);
	fwrite(portStr.c_str(), sizeof(char), portStr.size(), logfile);
	fwrite(clientIP.c_str(), sizeof(char), clientIP.size(), logfile);
}

void putClientOnDiferrentPort(SOCKET clientInstance, int port)
{
	char buffer[1024];
	int recvMsgSize;
	int sendResult;
	do
	{
		recvMsgSize = recv(clientInstance, buffer, 1024, 0);
		if (recvMsgSize == SOCKET_ERROR)
		{
			std::cerr << "recv failed: " << WSAGetLastError() << '\n';
		}
	} while (recvMsgSize > 0);
	sendResult = shutdown(clientInstance, SD_RECEIVE);
	if (sendResult == SOCKET_ERROR)
	{
		std::cerr << "Shutdown failed with error: " << WSAGetLastError() << '\n';
		closesocket(clientInstance);
		WSACleanup();
	}
	std::string portS = std::to_string(port);
	sendResult = send(clientInstance, portS.c_str(), (int)portS.size(), 0);
	if (sendResult == SOCKET_ERROR)
	{
		std::cerr << "Send failed with error: " << WSAGetLastError() << '\n';
		closesocket(clientInstance);
		WSACleanup();
	}
	sendResult = shutdown(clientInstance, SD_SEND);	//shutdown the connection since no more data will be sent
	if (sendResult == SOCKET_ERROR)
	{
		std::cerr << "shutdown failed with error: " << WSAGetLastError() << '\n';
		closesocket(clientInstance);
		WSACleanup();
	}
	sendResult = closesocket(clientInstance);	// close the socket
	if (sendResult == SOCKET_ERROR)
	{
		std::cerr << "close failed with error: " <<  WSAGetLastError() << '\n';
		WSACleanup();
	}
}

void getClientResource(SOCKET clientInstance)
{
	std::string filePath("");
	char buffer[1024];
	int recvMsgSize, bufError;
	do
	{
		recvMsgSize = recv(clientInstance, buffer, 1024, 0);
		if (recvMsgSize > 0)
		{
			filePath = processRequest(buffer);
			openFileWithPathAndSend(filePath, clientInstance);
			bufError = shutdown(clientInstance, SD_SEND);		// shutdown the connection since no more data will be sent
			if (bufError == SOCKET_ERROR)
			{
				std::cerr << "shutdown failed with error: " << WSAGetLastError() << '\n';
				closesocket(clientInstance);
				WSACleanup();
			}
		}
		else if (recvMsgSize < 0)
		{
			std::cerr << "recv failed: " << WSAGetLastError() << '\n';
		}
	} while (recvMsgSize > 0);
	bufError = shutdown(clientInstance, SD_RECEIVE);
	if (bufError == SOCKET_ERROR)
	{
		std::cerr << "shutdown failed with error: " << WSAGetLastError() << '\n';
		closesocket(clientInstance);
		WSACleanup();
	}
	bufError = closesocket(clientInstance);		// close the socket
	if (bufError == SOCKET_ERROR)
	{
		std::cerr << "close failed with error: " << WSAGetLastError() << '\n';
		WSACleanup();
	}
}

void openFileWithPathAndSend(std::string filePath, SOCKET clientInstance)
{
	FILE* file;
	errno_t err;
	err = fopen_s(&file, filePath.c_str(), "r");
	if (err == 0)
	{
		sendFile(file, clientInstance);
	}
	else
	{
		std::string responseNotFound = "HTTP/1.0 404 Not Found \r\n";
		std::cout << "Client response: " << responseNotFound;
		responseNotFound.append("Content-Type: text/html \r\n");
		responseNotFound.append("<HTML><HEAD><TITLE>Not Found lukas</TITLE></HEAD><BODY>Not Found</BODY></HTML>");
		send(clientInstance, responseNotFound.c_str(), (int)responseNotFound.size(), 0);
	}
	if (file)
	{
		err = fclose(file);
		if (err != 0)
		{
			std::cerr << "The file was not closed\n";
		}
	}
}

void sendFile(FILE* m_file, SOCKET clientInstance)
{
	char statusLine[] = "HTTP/1.0 200 OK\r\n";
	char contentTypeLine[] = "Content-Type: text/html\r\n";

	fseek(m_file, 0, SEEK_END);
	int bufferSize = ftell(m_file);
	rewind(m_file);
	
	std::unique_ptr<char[]> myBufferedFile = std::make_unique<char[]>(bufferSize);	// this creates unique pointer to my array 

	int numRead = fread_s(myBufferedFile.get(), bufferSize, sizeof(char), bufferSize, m_file);	//this reads whole file into buffer.

	int totalSend = bufferSize + strlen(statusLine) + strlen(contentTypeLine);

	std::unique_ptr<char[]> myUniqueBufferToSend = std::make_unique<char[]>(totalSend);

	std::memcpy(myUniqueBufferToSend.get(), &statusLine, strlen(statusLine));
	std::memcpy(myUniqueBufferToSend.get() + strlen(statusLine), &contentTypeLine, strlen(contentTypeLine));
	std::memcpy(myUniqueBufferToSend.get() + strlen(statusLine) + strlen(contentTypeLine), myBufferedFile.get(), bufferSize);

	int iResult = send(clientInstance, myUniqueBufferToSend.get(), totalSend, 0);
	if (iResult == SOCKET_ERROR)
	{
		std::cerr << "send failed with error: " << WSAGetLastError() << '\n';
		closesocket(clientInstance);
		WSACleanup();
	}
	std::cout << "Client response: " << statusLine << '\n';
}

std::string processRequest(char* bufferPtr)
{
	std::string firstLine("");
	while (*bufferPtr != '\r')	//extract the first line from buffer
	{
		firstLine += *bufferPtr;
		bufferPtr++;
	}
	std::cout << "\nClient request: " << firstLine << '\n';
	return getFilePath(firstLine);
}

//function to get the file that client want from http request using regex
static const std::string REGEX_GET = R"((GET)\s\/(.+)\s(HTTP.+))";
std::string getFilePath(std::string toParse)
{
	std::regex rx(REGEX_GET);
	std::string extractedSubmatchPath("");
	std::smatch pieces_match;
	if (std::regex_match(toParse, pieces_match, rx))
	{
		extractedSubmatchPath = pieces_match[2].str();
	}
	return extractedSubmatchPath;	//if there is no match so the request is not HTTP it will return empty string
}

//support function to check what is in the buffer
void printBuffer(char* bufferPtr, int size)
{
	for (int i = 0; i < size; i++)
	{
		std::cout << *bufferPtr;
		bufferPtr++;
	}
}