#pragma once
#include "../include/HttpRequest.h"
#include "Header.h"
#include "View.h"
#include <winsock2.h>
#include <fstream>
#include <vector>
#include <mutex>

class Parser
{
public:
	static std::string getClientData(SOCKET clientInstance, int port, int clientID);
	static Request parseRequestData(char* toParse, std::mutex& lock);
	static rMethod getRequestMethod(const std::string method);
	static View* urlIsAvailable(std::vector<View*> views, const std::string url);
	static std::string getIP(SOCKET socket);
};
