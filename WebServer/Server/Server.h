#pragma once
#include <winsock2.h>
#include <string>
#include <fstream>
#include <mutex>

class HttpServer
{
public:
	HttpServer();
	void startServer();

private:
	std::mutex lockPrint;
	int clientId;
	int portNumber;
	int clientNum;

	void startThread(const int port, std::ofstream& logFile);
	void serveClient(SOCKET clientInstance, int port, std::ofstream& logfile);
	void processRequest(SOCKET clientInstance);
	void sendResponse(std::string filePath, SOCKET clientInstance);
	void sendFile(const std::string html, SOCKET clientInstance);
};
