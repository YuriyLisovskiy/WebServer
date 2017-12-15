#pragma once
#include "../Utils/Request.h"
#include <winsock2.h>
#include <fstream>
#include <vector>

class HttpParser
{
public:
	static std::string getClientData(SOCKET clientInstance, int port, int clientID);
	static std::string parseRequestData(char* toParse);
private:
	static std::string getFirstLine(char*& bufferPtr);
	static std::string parseUrl(const std::string url);
};
