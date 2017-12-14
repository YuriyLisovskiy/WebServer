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
	int g_counter = 1;
	int g_clientId = 0;

	void start();
	void startThread(const int port, FILE* logFile);
	void serveClient(SOCKET clientInstance, int port, FILE* logfile);
	void printBuffer(char* bufferPtr, int size);
	void putClientOnDiferrentPort(SOCKET clientInstance, int port);
	void getClientResource(SOCKET clientInstance);
	void openFileWithPathAndSend(std::string filePath, SOCKET clientInstance);
	void sendFile(FILE* file, SOCKET clientInstance);
	std::string processRequest(char* bufferPtr);
	std::string getFilePath(std::string toParse);
	void getClientPortAndIP(SOCKET clientInstance, int port, FILE* logfile);
	int getNextPort(int port);
};
