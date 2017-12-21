#pragma once
#include "View.h"
#include "ServerMacros.h"
#include <string>
#include <fstream>
#include <mutex>
#include <vector>

class HttpServer
{
public:
	HttpServer();
	void run();
	void setView(View* view = new View());
	void setViews(std::vector<View*> views);

private:
	std::mutex lockPrint;
	int clientId;
	int portNumber;
	int clientNum;
	std::vector<View*> views;

	void startThread(const int port, std::ofstream& logFile);
	void serveClient(SOCK client, int port, std::ofstream& logfile);
	void processRequest(SOCK client);
	void sendResponse(Request& request, SOCK client);
	void sendFile(const std::string html, SOCK client);

	class Parser
	{
	public:
		static std::string getClientData(SOCK client, int port, int clientID);
		static View* urlIsAvailable(std::vector<View*> views, const std::string url);
		static std::string getIP(SOCK socket);
	};
};
