#pragma once

#ifndef WEB_SERVER_HTTP_SERVER_H
#define WEB_SERVER_HTTP_SERVER_H

#include "BaseView.h"
#include "ServerMacros.h"
#include "SimpleDB.h"
#include <string>
#include <fstream>
#include <mutex>
#include <vector>

__HTTP_BEGIN

class HttpServer
{
public:
	explicit HttpServer(SimpleDB* db = nullptr);
	void run();
	void setView(BaseView* view = new BaseView());
	void setViews(std::vector<BaseView*> views);

private:
	std::mutex lockPrint;
	int clientId;
	int portNumber;
	int clientNum;
	std::vector<BaseView*> views;
	SimpleDB* db;

	void startThread(const int port, std::ofstream& logFile);
	void serveClient(SOCK client, int port, std::ofstream& logfile);
	void processRequest(SOCK client);
	void sendResponse(Request& request, SOCK client);
	void sendFile(const std::string html, SOCK client);

	class Parser
	{
	public:
		static std::string getClientData(SOCK client, int port, int clientID);
		static BaseView* urlIsAvailable(std::vector<BaseView*> views, const std::string url);
		static std::string getIP(SOCK socket);
	};
};

__HTTP_END

#endif
