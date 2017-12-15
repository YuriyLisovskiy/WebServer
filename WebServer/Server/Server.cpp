#include "Server.h"
#include "../BL/Parser.h"
#include "../Utils/HtmlResponse.h"
#include <thread>
#include <iostream>

HttpServer::HttpServer()
{
	this->clientId = 0;
	this->clientNum = 0;
	this->portNumber = START_PORT;
}

void HttpServer::startServer()
{
	std::ofstream logFile("logFile.txt", std::ios::app);
	if (logFile.is_open())
	{
		std::cout << "Log file was created and opened.\n";
	}
	else
	{
		std::cerr << "Error\n";
	}
	int status;
	WSADATA wsaData;
	status = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (status != 0)
	{
		std::cerr << "WSAStartup failed: " << status << '\n';
		throw std::exception("WSAStartup failed: " + status + '\n');
	}
	std::thread newThread(&HttpServer::startThread, this, START_PORT, ref(logFile));
	if (newThread.joinable())
	{
		newThread.join();
	}
	logFile.close();
	WSACleanup();
	std::cin.ignore();
}

void HttpServer::startThread(const int port, std::ofstream& logFile)
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
	int breakAfterServed = 100; //this is the number of clients that will be served by the server
	bool listening = true;
	while (listening)
	{
		//if you want to change the number of clients than you have to change the number when server stop running
		if (this->clientNum == breakAfterServed)
		{
			std::chrono::milliseconds duration(2000);
			std::this_thread::sleep_for(duration);
			break;
		}
		else if ((client = accept(listenSock, (sockaddr*)&addr, &sa_size)) != INVALID_SOCKET)
		{
			std::thread newClient(&HttpServer::serveClient, this, client, this->portNumber++, std::ref(logFile));
			newClient.detach();
			this->clientId++;
			this->clientNum++;
		}
		else
		{
			std::cerr << "Invalid client socket\n";
			std::cin.get();
		}
	}
}

void HttpServer::serveClient(SOCKET clientInstance, int port, std::ofstream& logfile)
{
	lockPrint.lock();
	logfile << HttpParser::getClientData(clientInstance, port, this->clientId);
	lockPrint.unlock();
	clock_t start, finish;
	start = clock();
	this->processRequest(clientInstance);	//this function will get the webpage for client or page not found
	finish = clock();
	float servingTime = (float)(finish - start) / CLOCKS_PER_SEC;
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	localtime_s(&tstruct, &now);
	strftime(buf, sizeof(buf), "%a %b %d %Y [%r]", &tstruct);
	lockPrint.lock();
	logfile << buf << "\nRequest took: " + std::to_string(servingTime) + " seconds.\n";
	lockPrint.unlock();
}

void HttpServer::processRequest(SOCKET clientInstance)
{
	std::string filePath("");
	char buffer[1024];
	int recvMsgSize, bufError;
	do
	{
		recvMsgSize = recv(clientInstance, buffer, 1024, 0);
		if (recvMsgSize > 0)
		{
			std::string firstLine(HttpParser::getRequestData(buffer));
			this->lockPrint.lock();
			std::cout << "\nClient request: " << firstLine << '\n';
			this->lockPrint.unlock();
			filePath = HttpParser::parseRequestData(firstLine);
			openFileWithPathAndSend(filePath, clientInstance);
			bufError = shutdown(clientInstance, SD_SEND);		// shutdown the connection since no more data will be sent
			if (bufError == SOCKET_ERROR)
			{
				this->lockPrint.lock();
				std::cerr << "shutdown failed with error: " << WSAGetLastError() << '\n';
				this->lockPrint.unlock();
				closesocket(clientInstance);
				WSACleanup();
			}
		}
		else if (recvMsgSize < 0)
		{
			this->lockPrint.lock();
			std::cerr << "recv failed: " << WSAGetLastError() << '\n';
			this->lockPrint.unlock();
		}
	} while (recvMsgSize > 0);
	bufError = shutdown(clientInstance, SD_RECEIVE);
	if (bufError == SOCKET_ERROR)
	{
		this->lockPrint.lock();
		std::cerr << "shutdown failed with error: " << WSAGetLastError() << '\n';
		this->lockPrint.unlock();
		closesocket(clientInstance);
		WSACleanup();
	}
	bufError = closesocket(clientInstance);		// close the socket
	if (bufError == SOCKET_ERROR)
	{
		this->lockPrint.lock();
		std::cerr << "close failed with error: " << WSAGetLastError() << '\n';
		this->lockPrint.unlock();
		WSACleanup();
	}
}

void HttpServer::openFileWithPathAndSend(std::string filePath, SOCKET clientInstance)
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
		this->lockPrint.lock();
		std::string responseNotFound(HTMLResponse::notFound());
		this->lockPrint.unlock();
		send(clientInstance, responseNotFound.c_str(), (int)responseNotFound.size(), 0);
	}
	if (file)
	{
		err = fclose(file);
		if (err != 0)
		{
			this->lockPrint.lock();
			std::cerr << "The file was not closed\n";
			this->lockPrint.unlock();
		}
	}
}

void HttpServer::sendFile(FILE* file, SOCKET clientInstance)
{
	std::string statusLine = "HTTP/1.0 200 OK\r\n";
	std::string contentTypeLine = "Content-Type: text/html\r\n";
	fseek(file, 0, SEEK_END);
	int bufferSize = ftell(file);
	rewind(file);
	std::unique_ptr<char[]> myBufferedFile = std::make_unique<char[]>(bufferSize);
	int numRead = fread_s(myBufferedFile.get(), bufferSize, sizeof(char), bufferSize, file);
	int totalSend = bufferSize + statusLine.size() + contentTypeLine.size();
	std::unique_ptr<char[]> myUniqueBufferToSend = std::make_unique<char[]>(totalSend);
	std::memcpy(myUniqueBufferToSend.get(), statusLine.c_str(), statusLine.size());
	std::memcpy(myUniqueBufferToSend.get() + statusLine.size(), &contentTypeLine, contentTypeLine.size());
	std::memcpy(myUniqueBufferToSend.get() + statusLine.size() + contentTypeLine.size(), myBufferedFile.get(), bufferSize);
	int iResult = send(clientInstance, myUniqueBufferToSend.get(), totalSend, 0);
	if (iResult == SOCKET_ERROR)
	{
		this->lockPrint.lock();
		std::cerr << "send failed with error: " << WSAGetLastError() << '\n';
		this->lockPrint.unlock();
		closesocket(clientInstance);
		WSACleanup();
	}
	this->lockPrint.lock();
	std::cout << "Client response: " << statusLine << '\n';
	this->lockPrint.unlock();
}

//support function to check what is in the buffer
void HttpServer::printBuffer(char* bufferPtr, int size)
{
	for (int i = 0; i < size; i++)
	{
		std::cout << *bufferPtr;
		bufferPtr++;
	}
}
