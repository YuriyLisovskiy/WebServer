#include "../include/HttpServer.h"
#include "../include/HttpResponse.h"
#include "../include/Header.h"
#include <thread>
#include <iostream>
#include <ws2tcpip.h>

HttpServer::HttpServer()
{
	this->clientId = 0;
	this->clientNum = 0;
	this->portNumber = START_PORT;
	this->setView();
}

void HttpServer::setView(View* view)
{
	if (view)
	{
		this->views.clear();
		this->views.push_back(view);
	}
}

void HttpServer::setViews(std::vector<View*> views)
{
	this->views.clear();
	for (const auto& view : views)
	{
		if (view)
		{
			this->views.push_back(view);
		}
	}
}

void HttpServer::run()
{
	std::ofstream logFile("logFile.txt", std::ios::app);
	if (!logFile.is_open())
	{
		std::cerr << "SERVER ERROR: 'HttpServer::run()': file 'logFile.txt' is not opened.\n";
	}
	int status;
	WSADATA wsaData;
	status = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (status != 0)
	{
		std::cerr << "SERVER ERROR: 'HttpServer::run()': 'WSAStartup(MAKEWORD(2, 2), &wsaData)' failed with error #" << status << '\n';
		return;
	}
	std::thread newThread(&HttpServer::startThread, this, START_PORT, ref(logFile));
	if (newThread.joinable())
	{
		newThread.join();
	}
	logFile.close();
	WSACleanup();
}

void HttpServer::startThread(const int port, std::ofstream& logFile)
{
	SOCKET listenSock;
	SOCKET client;
	int status;
	sockaddr_in addr;
	socklen_t sa_size = sizeof(sockaddr_in);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	inet_pton(AF_INET, SERVER_IP, &(addr.sin_addr));
	listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSock == INVALID_SOCKET)
	{
		std::cerr << "SERVER ERROR: 'HttpServer::startThread()': 'socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)' failed with error #" << WSAGetLastError() << '\n';
		WSACleanup();
		return;
	}
	status = bind(listenSock, (sockaddr*)&addr, sizeof(sockaddr_in));
	if (status == SOCKET_ERROR)
	{
		std::cerr << "SERVER ERROR: 'HttpServer::startThread()': 'bind(listenSock, (sockaddr*)&addr, sizeof(sockaddr_in))' failed with error #" << WSAGetLastError() << '\n';
		status = closesocket(listenSock);
		if (status == SOCKET_ERROR)
		{
			std::cerr << "SERVER ERROR: 'HttpServer::startThread()': 'closesocket(listenSock)' failed with error #" << WSAGetLastError() << '\n';
		}
		WSACleanup();
		return;
	}
	if (listen(listenSock, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cerr << "SERVER ERROR: 'HttpServer::startThread()': 'listen(listenSock, SOMAXCONN)' failed with error #" << WSAGetLastError() << '\n';
		return;
	}
	bool listening = true;
	PRINT_SERVER_DATA(std::cout, Parser::getIP(listenSock), START_PORT);
	while (listening)
	{
		if (this->clientNum == MAX_SERVERD)
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
		}
		else
		{
			std::cerr << "SERVER ERROR: 'HttpServer::startThread()': invalid client socket.\n";
		}
	}
}

void HttpServer::serveClient(SOCKET client, int port, std::ofstream& logfile)
{
	lockPrint.lock();
	logfile << Parser::getClientData(client, port, this->clientId);
	lockPrint.unlock();
	clock_t start, finish;
	start = clock();
	this->processRequest(client);
	finish = clock();
	float servingTime = (float)(finish - start) / CLOCKS_PER_SEC;
	if (logfile.is_open())
	{
		this->lockPrint.lock();
		DATE_TIME_NOW(logfile);
		logfile << "\nRequest took: " + std::to_string(servingTime) + " seconds.\n";
		this->lockPrint.unlock();
	}
}

void HttpServer::processRequest(SOCKET client)
{
	char buffer[MAX_BUFF_SIZE];
	int recvMsgSize, bufError;
	std::string data("");
	size_t size = 0;
	do
	{
		recvMsgSize = recv(client, buffer, MAX_BUFF_SIZE, 0);
		if (recvMsgSize > 0)
		{
			data += buffer;
			size += recvMsgSize;
			data = data.substr(0, size);
		}
		else if (recvMsgSize < 0)
		{
			this->lockPrint.lock();
			std::cerr << "SERVER ERROR: 'HttpServer::processRequest()': 'recv(client, buffer, 1024, 0)' failed with error #" << WSAGetLastError() << '\n';
			this->lockPrint.unlock();
		}
	} while (recvMsgSize >= MAX_BUFF_SIZE);
	try
	{
		Request request(Request::Parser::parseRequestData((char*)data.c_str(), this->lockPrint));
		this->sendResponse(request, client);
	}
	catch (...)
	{
		this->sendFile(Response::BadRequest(), client);
	}
	bufError = shutdown(client, SD_SEND);
	if (bufError == SOCKET_ERROR)
	{
		this->lockPrint.lock();
		std::cerr << "SERVER ERROR: 'HttpServer::processRequest()': 'shutdown(client, SD_SEND)' failed with error #" << WSAGetLastError() << '\n';
		this->lockPrint.unlock();
		closesocket(client);
		WSACleanup();
	}

	bufError = shutdown(client, SD_RECEIVE);
	if (bufError == SOCKET_ERROR)
	{
		this->lockPrint.lock();
		std::cerr << "SERVER ERROR: 'HttpServer::processRequest()': 'shutdown(client, SD_RECEIVE)' failed with error #" << WSAGetLastError() << '\n';
		this->lockPrint.unlock();
		closesocket(client);
		WSACleanup();
	}
	bufError = closesocket(client);
	if (bufError == SOCKET_ERROR)
	{
		this->lockPrint.lock();
		std::cerr << "SERVER ERROR: 'HttpServer::processRequest()': 'closesocket(client)' failed with error #" << WSAGetLastError() << '\n';
		this->lockPrint.unlock();
		WSACleanup();
	}
}

void HttpServer::sendResponse(Request& request, SOCKET clientInstance)
{
	std::string html("");
	View* view = Parser::urlIsAvailable(this->views, request.DATA.get("url"));
	if (view)
	{
		switch (Request::Parser::getRequestMethod(request.DATA.get("method")))
		{
		case REQUEST_METHOD::GET:
			html = view->Get(request);
			break;
		case REQUEST_METHOD::POST:
			html = view->Post(request);
			break;
		case REQUEST_METHOD::PUT:
			html = view->Put(request);
			break;
		case REQUEST_METHOD::DElETE:
			html = view->Delete(request);
			break;
		default:
			html = Response::MethodNotAllowed();
			break;
		}
	}
	else
	{
		html = Response::NotFound();
	}
	this->sendFile(html, clientInstance);
}

void HttpServer::sendFile(const std::string httpResponse, SOCKET client)
{
	if (send(client, httpResponse.c_str(), (int)httpResponse.size(), 0) == SOCKET_ERROR)
	{
		this->lockPrint.lock();
		std::cerr << "SERVER ERROR: 'HttpServer::sendFile()': 'send(client, httpResponse.c_str(), (int)httpResponse.size(), 0) failed with error #" << WSAGetLastError() << '\n';
		this->lockPrint.unlock();
		closesocket(client);
		WSACleanup();
	}
}
