#pragma once

#ifndef WEB_SERVER_SERVER_H
#define WEB_SERVER_SERVER_H

#include "headers.h"
#include "request.h"
#include "application.h"
#include <mutex>
#include <vector>
#include <queue>

__HTTP_BEGIN

class Server
{
private:
	std::mutex lockPrint;
	Application* app;
	uint16_t port;
	std::queue<SOCK> clientsQueue;
	void startListener();
	void serveClient(const SOCK client);
	void processRequest(const SOCK& client);
	void sendResponse(Request& request, const SOCK& client);
	void sendFile(const std::string& html, const SOCK& client);
	void closeSocket(const SOCK& sock, const int how, const std::string& method, const std::string& func, const int line);
	class Parser
	{
	public:
		static std::string getClientData(const SOCK& client, uint16_t port, int clientID);
		static std::string getIP(const SOCK& socket);
		static bool requestStatic(const std::string& url);
	};
public:
	explicit Server(const std::string& port = SERVER_PORT);
	void start();
	void setApp(Application* app = new Application());
};

__HTTP_END

#endif
