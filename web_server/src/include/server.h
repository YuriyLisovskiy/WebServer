#pragma once

#ifndef WEB_SERVER_SERVER_H
#define WEB_SERVER_SERVER_H

#include "headers.h"
#include "request.h"
#include "view.h"
#include <mutex>
#include <vector>

__HTTP_BEGIN

class Server
{
private:
	std::mutex lockPrint;
	std::vector<View*> views;
	uint16_t port;
	void startThread(std::ofstream& logFile);
	void serveClient(SOCK client, std::ofstream& logfile);
	void processRequest(SOCK client);
	void sendResponse(Request& request, SOCK client);
	void sendFile(const std::string html, SOCK client);
	void closeSocket(SOCK& sock, int how, const std::string method, const std::string func, const int line);
	void logData(std::ofstream& logfile, const float servingTime);
	void printErr(const std::string msg, const int line);
	class Parser
	{
	public:
		static std::string getClientData(SOCK client, uint16_t port, int clientID);
		static View* availableView(std::vector<View*> views, const std::string url);
		static std::string getIP(SOCK socket);
		static bool requestStatic(const std::string url);
	};
public:
	explicit Server(const std::string port = SERVER_PORT);
	void start();
	void setView(View* view = new View());
	void setViews(std::vector<View*> views);
};

__HTTP_END

#endif
