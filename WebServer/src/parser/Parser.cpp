#include "../include/Parser.h"
#include "../include/RegularExpressions.h"
#include <ws2tcpip.h>
#include <regex>
#include <iostream>

std::string Parser::getClientData(SOCKET client, int port, int clientID)
{
	std::string result("ID: " + std::to_string(clientID) + "\nThe Client port is: " + 
		std::to_string(port) + "\nThe Client IP is: " + Parser::getIP(client) + '\n');
	return result;
}

Request Parser::parseRequestData(char* toParse, std::mutex& lock)
{
	std::string firstLine("");
	while (*toParse != '\r')
	{
		firstLine += *toParse;
		toParse++;
	}
	toParse += 2;
	lock.lock();
	std::cout << "\n";
	DATE_TIME_NOW(std::cout);
	std::cout << " " << firstLine << '\n';
	lock.unlock();
	std::string url(""), method("");
	std::smatch data;
	if (std::regex_match(firstLine, data, std::regex(REGEX::FIRST_LINE_REQUEST)))
	{
		method = data[1].str();
		url = data[2].str();
	}
	std::string body(toParse);
	body = std::regex_replace(body, std::regex("\\r+"), "");
	return Request(body, method, url);
}

rMethod Parser::getRequestMethod(const std::string method)
{
	rMethod result = rMethod::None;
	std::string methodToLower(method);
	std::transform(methodToLower.begin(), methodToLower.end(), methodToLower.begin(), ::tolower);
	if (methodToLower == "get")
	{
		result = rMethod::Get;
	}
	else if (methodToLower == "post")
	{
		result = rMethod::Post;
	}
	else if (methodToLower == "put")
	{
		result = rMethod::Put;
	}
	else if (methodToLower == "delete")
	{
		result = rMethod::Delete;
	}
	return result;
}

View* Parser::urlIsAvailable(std::vector<View*> views, const std::string url)
{
	for (const auto& view : views)
	{
		if (view->hasUrl(url))
		{
			return view;
		}
	}
	return nullptr;
}

std::string Parser::getIP(SOCKET socket)
{
	struct sockaddr_in addr;
	socklen_t addr_size = sizeof(struct sockaddr_in);
	int res = getsockname(socket, (struct sockaddr *)&addr, &addr_size);
	sockaddr_in* pV4Addr = (struct sockaddr_in*)&addr;
	int ipAddr = pV4Addr->sin_addr.s_addr;
	char clientIp[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &ipAddr, clientIp, INET_ADDRSTRLEN);
	return clientIp;
}
