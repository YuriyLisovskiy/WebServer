#pragma once
#include "../Utils/Header.h"

class HttpServer
{
public:
	HttpServer();
	void startServer();

private:
	std::mutex lockPrint;
	std::mutex lockCounter;
	int clientId;
	int portNumber;
	int clientNum;

	void startThread(const int port, std::ofstream& logFile);
	void serveClient(SOCKET clientInstance, int port, std::ofstream& logfile);
	void printBuffer(char* bufferPtr, int size);
	void processRequest(SOCKET clientInstance);
	void openFileWithPathAndSend(std::string filePath, SOCKET clientInstance);
	void sendFile(FILE* file, SOCKET clientInstance);
};
