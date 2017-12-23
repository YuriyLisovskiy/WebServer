#include "../include/HttpServer.h"
#include "../include/HttpResponse.h"
#include <thread>
#include <iostream>
#include <sstream>

HTTP::HttpServer::HttpServer(SimpleDB* db)
{
	this->clientId = 0;
	this->clientNum = 0;
	this->portNumber = START_PORT;
	this->setView();
	this->db = db;
}

void HTTP::HttpServer::setView(BaseView* view)
{
	if (view)
	{
		this->views.clear();
		this->views.push_back(view);
	}
}

void HTTP::HttpServer::setViews(std::vector<BaseView*> views)
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

void HTTP::HttpServer::run()
{
	std::ofstream logFile;
	logFile.open(BASE_DIR + "test/web_site/log.txt", std::ios::app | std::ios::out);
	if (!logFile.is_open())
	{
		std::cerr << "SERVER ERROR: 'HttpServer::run()': file 'log.txt' is not opened.\n";
	}
	WSA_STARTUP;
	std::thread newThread(&HttpServer::startThread, this, START_PORT, ref(logFile));
	if (newThread.joinable())
	{
		newThread.join();
	}
	logFile.close();
	WSA_CLEANUP;
}

void HTTP::HttpServer::startThread(const int port, std::ofstream& logFile)
{
	SOCK listenSock;
	SOCK client;
	int status;
	sockaddr_in addr;
	socklen_t sa_size = sizeof(sockaddr_in);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
//	addr.sin_addr.s_addr = htonl(SERVER_IP);
	inet_pton(AF_INET, SERVER_IP, &(addr.sin_addr));
	listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSock == INVALID_SOCK)
	{
		std::cerr << "SERVER ERROR: 'HttpServer::startThread()': 'socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)'" << WSA_LAST_ERR;
		WSA_CLEANUP;
		return;
	}
	status = bind(listenSock, (sockaddr*)&addr, sizeof(sockaddr_in));
	if (status == SOCK_ERROR)
	{
		std::cerr << "SERVER ERROR: 'HttpServer::startThread()': 'bind(listenSock, (sockaddr*)&addr, sizeof(sockaddr_in))'" << WSA_LAST_ERR;
		status = CLOSE_SOCK(listenSock);
		if (status == SOCK_ERROR)
		{
			std::cerr << "SERVER ERROR: 'HttpServer::startThread()': 'closesocket(listenSock)'" << WSA_LAST_ERR;
		}
		WSA_CLEANUP;
		return;
	}
	if (listen(listenSock, SOMAXCONN) == SOCK_ERROR)
	{
		std::cerr << "SERVER ERROR: 'HttpServer::startThread()': 'listen(listenSock, SOMAXCONN)'" << WSA_LAST_ERR;
		return;
	}
	bool listening = true;
	PRINT_SERVER_DATA(std::cout, Parser::getIP(listenSock), START_PORT);
	while (listening)
	{
		if (this->clientNum == MAX_SERVED)
		{
			std::chrono::milliseconds duration(2000);
			std::this_thread::sleep_for(duration);
			break;
		}
		else if ((client = accept(listenSock, (sockaddr*)&addr, &sa_size)) != INVALID_SOCK)
		{
			std::thread newClient(&HttpServer::serveClient, this, client, this->portNumber++, std::ref(logFile));
			newClient.detach();
			this->clientId++;
			this->clientNum++;
		}
		else
		{
			std::cerr << "SERVER ERROR: 'HttpServer::startThread()': invalid client socket.\n";
		}
	}
}

void HTTP::HttpServer::serveClient(SOCK client, int port, std::ofstream& logfile)
{
	if (this->db)
	{
		lockPrint.lock();
		this->db->write({ "clients", Parser::getIP(client) }, true);
		lockPrint.unlock();
	}
	clock_t start, finish;
	start = clock();
	this->processRequest(client);
	finish = clock();
	float servingTime = (float)(finish - start) / CLOCKS_PER_SEC;
	if (logfile.is_open())
	{
		this->lockPrint.lock();
		logfile << '[';
		DATE_TIME_NOW(logfile);
		logfile << ']';
		logfile << "\nRequest took: " + std::to_string(servingTime) + " seconds.\n\n";
		this->lockPrint.unlock();
	}
	this->lockPrint.lock();
	std::stringstream ss;
	DATE_TIME_NOW(ss);
	this->db->write({ "statistic_date", ss.str() });
	this->db->write({ "statistic_speed", std::to_string(servingTime) + " second(s)"});
	this->lockPrint.unlock();
}

void HTTP::HttpServer::processRequest(SOCK client)
{
	char buffer[MAX_BUFF_SIZE];
	int recvMsgSize, bufError;
	std::string data;
	int size = 0;
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
			std::cerr << "SERVER ERROR: 'HttpServer::processRequest()': 'recv(client, buffer, 1024, 0)'" << WSA_LAST_ERR;
			this->lockPrint.unlock();
		}
	} while (recvMsgSize >= MAX_BUFF_SIZE);
	try
	{
		Request request(Request::Parser::parseRequestData((char*)data.c_str(), this->lockPrint, Parser::getIP(client)));
		this->sendResponse(request, client);
	}
	catch (...)
	{
		this->sendFile(Response::BadRequest(), client);
	}
	bufError = shutdown(client, SOCK_SEND);
	if (bufError == SOCK_ERROR)
	{
		this->lockPrint.lock();
		std::cerr << "SERVER ERROR: 'HttpServer::processRequest()': 'shutdown(client, SD_SEND)'" << WSA_LAST_ERR;
		this->lockPrint.unlock();
		CLOSE_SOCK(client);
		WSA_CLEANUP;
	}

	bufError = shutdown(client, SOCK_RECEIVE);
	if (bufError == SOCK_ERROR)
	{
		this->lockPrint.lock();
		std::cerr << "SERVER ERROR: 'HttpServer::processRequest()': 'shutdown(client, SD_RECEIVE)'" << WSA_LAST_ERR;
		this->lockPrint.unlock();
		CLOSE_SOCK(client);
		WSA_CLEANUP;
	}
	bufError = CLOSE_SOCK(client);
	if (bufError == SOCK_ERROR)
	{
		this->lockPrint.lock();
		std::cerr << "SERVER ERROR: 'HttpServer::processRequest()': 'closesocket(client)'" << WSA_LAST_ERR;
		this->lockPrint.unlock();
		WSA_CLEANUP;
	}
}

void HTTP::HttpServer::sendResponse(Request& request, SOCK clientInstance)
{
	std::string html;
	BaseView* view = Parser::urlIsAvailable(this->views, request.DATA.get("url"));
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

void HTTP::HttpServer::sendFile(const std::string httpResponse, SOCK client)
{
	if (send(client, httpResponse.c_str(), (int)httpResponse.size(), 0) == SOCK_ERROR)
	{
		this->lockPrint.lock();
		std::cerr << "SERVER ERROR: 'HttpServer::sendFile()': 'send(client, httpResponse.c_str(), (int)httpResponse.size(), 0)'" << WSA_LAST_ERR;
		this->lockPrint.unlock();
		CLOSE_SOCK(client);
		WSA_CLEANUP;
	}
}
