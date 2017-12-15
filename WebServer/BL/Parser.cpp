#include "Parser.h"
#include "../Utils/RegularExpressions.h"
#include <ws2tcpip.h>
#include <regex>
#include <iostream>

std::string HttpParser::getClientData(SOCKET clientInstance, int port, int clientID)
{
	struct sockaddr_in addr;
	socklen_t addr_size = sizeof(struct sockaddr_in);
	int res = getsockname(clientInstance, (struct sockaddr *)&addr, &addr_size);
	sockaddr_in* pV4Addr = (struct sockaddr_in*)&addr;
	int ipAddr = pV4Addr->sin_addr.s_addr;
	char clientIp[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &ipAddr, clientIp, INET_ADDRSTRLEN);
	std::string result("ID: " + std::to_string(clientID) + "\nThe Client port is: " + 
		std::to_string(port) + "\nThe Client IP is: " + clientIp + '\n');
	return result;
}

std::string HttpParser::getFirstLine(char* bufferPtr)
{
	std::string firstLine("");
	while (*bufferPtr != '\r')
	{
		firstLine += *bufferPtr;
		bufferPtr++;
	}
	return firstLine;
}

std::string HttpParser::parseRequestData(char* toParse)
{
	std::string firstLine(HttpParser::getFirstLine(toParse));
	std::cout << "\nClient request: " << firstLine << '\n';
	std::smatch pieces_match;
	std::string url("");
	if (std::regex_match(firstLine, pieces_match, std::regex(REGEX::REQUEST_REGEX)))
	{
		url = pieces_match[2].str();
	}
	return url;
}
