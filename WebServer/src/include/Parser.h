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
	static std::string parseUrl(const std::string url, std::map<std::string, std::string>& container);
	static std::string parseValue(const std::string value);
	static void parseCookies(const std::string cookies, std::map<std::string, std::string>& container);
	static void parseHeaders(const std::string headers, std::map<std::string, std::string>& container, std::map<std::string, std::string>& cookiesContainer);
};
