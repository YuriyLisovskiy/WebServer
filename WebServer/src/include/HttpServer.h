#pragma once
#include "View.h"
#include <winsock2.h>
#include <string>
#include <fstream>
#include <mutex>
#include <vector>

class HttpServer
{
public:
	HttpServer();
	void startServer();
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
};
