#include "../include/server.h"
#include <thread>
#include <iostream>
#include <sstream>
#include <fstream>

http::Server::Server(const std::string& port)
{
	try
	{
		this->port = (uint16_t)std::stoi(port);
	}
	catch (const std::exception& exc)
	{
		this->printErr("'http::Server::Server()': " + std::string(exc.what()) + ".", __LINE__ - 4);
	}
	this->setView();
}
void http::Server::setView(View* view)
{
	if (view)
	{
		this->views.clear();
		this->views.push_back(view);
	}
}
void http::Server::setViews(std::vector<View*> views)
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
void http::Server::start()
{
	std::ofstream logFile(BASE_DIR + "log.txt", std::ios::app);
	if (!logFile.is_open())
	{
		this->printErr("'http::Server::run()': file 'log.txt' is not opened.", __LINE__ - 3);
	}
	WSA_STARTUP;
	std::thread newThread(&Server::startThread, this, ref(logFile));
	if (newThread.joinable())
	{
		newThread.join();
	}
	logFile.close();
	WSA_CLEANUP;
}
void http::Server::startThread(std::ofstream& logFile)
{
	SOCK listenSock;
	int status;
	sockaddr_in addr;
	socklen_t sa_size = sizeof(sockaddr_in);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(this->port);
	inet_pton(AF_INET, SERVER_IP, &(addr.sin_addr));
	if ((listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCK)
	{
		this->printErr("'http::Server::startThread()': 'socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)' failed.", __LINE__ - 2);
		WSA_CLEANUP;
		return;
	}
	if (bind(listenSock, (sockaddr*)&addr, sizeof(sockaddr_in)) == SOCK_ERROR)
	{
		this->printErr("'http::Server::startThread()': 'bind(listenSock, (sockaddr*)&addr, sizeof(sockaddr_in))' failed.", __LINE__ - 2);
		if (CLOSE_SOCK(listenSock) == SOCK_ERROR)
		{
			this->printErr("'http::Server::startThread()': 'closesocket(listenSock)' failed.", __LINE__ - 2);
		}
		WSA_CLEANUP;
		return;
	}
	if (listen(listenSock, SOMAXCONN) == SOCK_ERROR)
	{
		this->printErr("'http::Server::startThread()': 'listen(listenSock, SOMAXCONN)' failed.", __LINE__ - 2);
		return;
	}
	bool listening = true;
	PRINT_SERVER_DATA(std::cout, Parser::getIP(listenSock), this->port);
	SOCK client;
	while (listening)
	{
		if ((client = accept(listenSock, (sockaddr*)&addr, &sa_size)) != INVALID_SOCK)
		{
			std::thread newClient(&Server::serveClient, this, client, std::ref(logFile));
			newClient.detach();
		}
		else
		{
			this->printErr("'http::Server::startThread()': invalid client socket.", __LINE__ - 7);
		}
	}
}
void http::Server::serveClient(const SOCK client, std::ofstream& logfile)
{
	clock_t start, finish;
	start = clock();
	this->processRequest(client);
	finish = clock();
	if (logfile.is_open())
	{
		this->logData(logfile, (float)(finish - start) / CLOCKS_PER_SEC);
	}
}
void http::Server::processRequest(const SOCK& client)
{
	char buffer[MAX_BUFF_SIZE];
	MSG_SIZE recvMsgSize;
	int bufError;
	std::string data;
	unsigned long size = 0;
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
			this->printErr("'http::Server::processRequest()': 'recv(client, buffer, 1024, 0)' failed.", __LINE__ - 9);
		}
	} while (recvMsgSize >= MAX_BUFF_SIZE);
	try
	{
		std::string dataToParse(data.c_str());
		if (!dataToParse.empty())
		{
			Request request(Request::Parser::parseRequestData((char*)dataToParse.c_str(), this->lockPrint, Parser::getIP(client)));
			this->sendResponse(request, client);
		}
		else
		{
			this->sendFile(Response::BadRequest(), client);
		}
	}
	catch (...)
	{
		this->sendFile(Response::BadRequest(), client);
	}
	this->closeSocket(client, SOCK_SEND, "processRequest()", "shutdown(client, SOCK_SEND)", __LINE__);
	this->closeSocket(client, SOCK_RECEIVE, "processRequest()", "shutdown(client, SOCK_RECEIVE)", __LINE__);
	bufError = CLOSE_SOCK(client);
	if (bufError == SOCK_ERROR)
	{
		this->printErr("'http::Server::processRequest()': 'CLOSE_SOCK(client)' failed.", __LINE__ - 3);
		WSA_CLEANUP;
	}
}
void http::Server::sendResponse(Request& request, const SOCK& client)
{
	std::string response;
	std::string url = request.DATA.get("url");
	View* view = Parser::availableView(this->views, url);
	if (view)
	{
		if (Parser::requestStatic(url))
		{
			response = Response::responseStatic(view->createStaticDir(url));
		}
		else
		{
			switch (Request::Parser::getRequestMethod(request.DATA.get("method")))
			{
			case REQUEST_METHOD::GET:
				response = view->Get(request);
				break;
			case REQUEST_METHOD::POST:
				response = view->Post(request);
				break;
			case REQUEST_METHOD::PUT:
				response = view->Put(request);
				break;
			case REQUEST_METHOD::DElETE:
				response = view->Delete(request);
				break;
			case REQUEST_METHOD::HEAD:
				response = view->Head(request);
				break;
			case REQUEST_METHOD::CONNECT:
				response = view->Connect(request);
				break;
			case REQUEST_METHOD::OPTIONS:
				response = view->Options(request);
				break;
			case REQUEST_METHOD::PATCH:
				response = view->Patch(request);
				break;
			case REQUEST_METHOD::TRACE:
				response = view->Trace(request);
				break;
			default:
				response = Response::MethodNotAllowed();
				break;
			}
		}
	}
	else
	{
		response = Response::NotFound();
	}
	this->sendFile(response, client);
}
void http::Server::sendFile(const std::string& httpResponse, const SOCK& client)
{
	if (send(client, httpResponse.c_str(), (int)httpResponse.size(), 0) == SOCK_ERROR)
	{
		this->printErr("'http::Server::sendFile()': 'send(client, httpResponse.c_str(), (int)httpResponse.size(), 0)' failed.", __LINE__ - 2);
		if (CLOSE_SOCK(client) == SOCK_ERROR)
		{
			this->printErr("'http::Server::sendFile()': 'CLOSE_SOCK(client)' failed.", __LINE__ - 2);
		}
		WSA_CLEANUP;
	}
}
void http::Server::closeSocket(const SOCK& sock, const int how, const std::string& method, const std::string& func, const int line)
{
	if (shutdown(sock, how) == SOCK_ERROR)
	{
		this->printErr("'http::Server::" + method + "': '" + func + "' failed.", line);
		if (CLOSE_SOCK(sock) == SOCK_ERROR)
		{
			this->printErr("'http::Server::closeSocket()': 'CLOSE_SOCK(sock)' failed.", __LINE__ - 2);
		}
		WSA_CLEANUP;
	}
}
void http::Server::logData(std::ofstream& logfile, const float servingTime)
{
	this->lockPrint.lock();
	logfile << '[';
	DATE_TIME_NOW(logfile, "%d/%b/%Y %r");
	logfile << ']';
	logfile << "\nRequest took: " + std::to_string(servingTime) + " seconds.\n\n";
	this->lockPrint.unlock();
}
void http::Server::printErr(const std::string& msg, const int line)
{
	this->lockPrint.lock();
	std::cerr << "\nSERVER ERROR:\n Message: \"" << msg << "\"\n Line: " << line << '\n';
	this->lockPrint.unlock();
}
