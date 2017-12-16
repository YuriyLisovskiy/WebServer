#include "../include/HttpServer.h"
#include "../include/HttpParser.h"
#include "../include/HtmlResponse.h"
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
		this->view = view;
	}
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
	bool listening = true;
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
			this->clientNum++;
		}
		else
		{
			std::cerr << "Invalid client socket\n";
		}
	}
}

void HttpServer::serveClient(SOCKET client, int port, std::ofstream& logfile)
{
	lockPrint.lock();
	logfile << HttpParser::getClientData(client, port, this->clientId);
	lockPrint.unlock();
	clock_t start, finish;
	start = clock();
	this->processRequest(client);
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

void HttpServer::processRequest(SOCKET client)
{
	char buffer[1024];
	int recvMsgSize, bufError;
	do
	{
		recvMsgSize = recv(client, buffer, 1024, 0);
		if (recvMsgSize > 0)
		{
			this->lockPrint.lock();
			Request request(HttpParser::parseRequestData(buffer));
			this->lockPrint.unlock();
			this->sendResponse(request, client);
			bufError = shutdown(client, SD_SEND);
			if (bufError == SOCKET_ERROR)
			{
				this->lockPrint.lock();
				std::cerr << "shutdown failed with error: " << WSAGetLastError() << '\n';
				this->lockPrint.unlock();
				closesocket(client);
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
	bufError = shutdown(client, SD_RECEIVE);
	if (bufError == SOCKET_ERROR)
	{
		this->lockPrint.lock();
		std::cerr << "shutdown failed with error: " << WSAGetLastError() << '\n';
		this->lockPrint.unlock();
		closesocket(client);
		WSACleanup();
	}
	bufError = closesocket(client);
	if (bufError == SOCKET_ERROR)
	{
		this->lockPrint.lock();
		std::cerr << "close failed with error: " << WSAGetLastError() << '\n';
		this->lockPrint.unlock();
		WSACleanup();
	}
}

void HttpServer::sendResponse(Request& request, SOCKET clientInstance)
{
	std::string html("");
	if (this->view->hasUrl(request.DATA.get("url")))
	{
		switch (HttpParser::getRequestMethod(request.DATA.get("method")))
		{
		case rMethod::Get:
			html = this->view->Get(request);
			break;
		case rMethod::Post:
			html = this->view->Post(request);
			break;
		case rMethod::Put:
			html = this->view->Put(request);
			break;
		case rMethod::Delete:
			html = this->view->Delete(request);
			break;
		default:
			html = HTMLResponse::MethodNotAllowed();
			break;
		}
	}
	else
	{
		html = HTMLResponse::NotFound();
	}
	this->sendFile(html, clientInstance);
}

void HttpServer::sendFile(const std::string httpResponse, SOCKET client)
{
	if (send(client, httpResponse.c_str(), (int)httpResponse.size(), 0) == SOCKET_ERROR)
	{
		this->lockPrint.lock();
		std::cerr << "send failed with error: " << WSAGetLastError() << '\n';
		this->lockPrint.unlock();
		closesocket(client);
		WSACleanup();
	}
}
