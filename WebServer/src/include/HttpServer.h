#pragma once
#include "View.h"
#include "ServerMacros.h"
#include <string>
#include <fstream>
#include <mutex>
#include <vector>
#include <winsock2.h>

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
	void serveClient(SOCKET clientInstance, int port, std::ofstream& logfile);
	void processRequest(SOCKET clientInstance);
	void sendResponse(Request& request, SOCKET clientInstance);
	void sendFile(const std::string html, SOCKET clientInstance);

	class Parser
	{
	public:
		static std::string getClientData(SOCKET clientInstance, int port, int clientID);
		static View* urlIsAvailable(std::vector<View*> views, const std::string url);
		static std::string getIP(SOCKET socket);
	};
};
