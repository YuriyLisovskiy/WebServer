#pragma once
#include "../include/Request.h"
#include <winsock2.h>
#include <fstream>
#include <vector>

class HttpParser
{
public:
	static std::string getClientData(SOCKET clientInstance, int port, int clientID);
	static std::string parseRequestData(char* toParse);
};
