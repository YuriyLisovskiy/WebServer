#pragma once
#include <winsock2.h>
#include <fstream>

class HttpParser
{
public:
	static std::string getClientData(SOCKET clientInstance, int port, int clientID);
	static std::string getRequestData(char* bufferPtr);
	static std::string parseRequestData(std::string toParse);
};
