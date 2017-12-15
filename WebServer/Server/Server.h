#pragma once
#include "../Utils/Header.h"

class HttpServer
{
public:
	HttpServer();

private:
	std::mutex g_lockPrint;
	std::mutex g_lockCounter;
	std::recursive_mutex g_r_lock;
	int clientId = 0;
	int portNumber = START_PORT;
	int clientNum = 0;

	void start();
	void startThread(const int port, std::ofstream& logFile);
	void serveClient(SOCKET clientInstance, int port, std::ofstream& logfile);
	void printBuffer(char* bufferPtr, int size);
	void getClientResource(SOCKET clientInstance);
	void openFileWithPathAndSend(std::string filePath, SOCKET clientInstance);
	void sendFile(FILE* file, SOCKET clientInstance);
	std::string processRequest(char* bufferPtr);
	std::string getFilePath(std::string toParse);
	void getClientPortAndIP(SOCKET clientInstance, int port, std::ofstream& logfile);
};
