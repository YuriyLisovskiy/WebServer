#pragma once
#include <winsock2.h>
#include <fstream>

class HttpParser
{
public:
	static std::string getClientData(SOCKET clientInstance, int port, int clientID);
	static std::string parseRequestData(char* toParse);
private:
	static std::string getFirstLine(char* bufferPtr);
};
