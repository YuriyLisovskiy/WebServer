#include "Parser.h"
#include "../Utils/RegularExpressions.h"
#include <ws2tcpip.h>
#include <regex>

std::string HttpParser::getClientData(SOCKET clientInstance, int port, int clientID)
{
	struct sockaddr_in addr;
	socklen_t addr_size = sizeof(struct sockaddr_in);
	int res = getsockname(clientInstance, (struct sockaddr *)&addr, &addr_size);
	sockaddr_in* pV4Addr = (struct sockaddr_in*)&addr;
	int ipAddr = pV4Addr->sin_addr.s_addr;
	char clientIp[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &ipAddr, clientIp, INET_ADDRSTRLEN);
	return ("ID: " + std::to_string(clientID) + "\nThe Client port is: " + std::to_string(port) + "\nThe Client IP is: " + clientIp + '\n');
}

std::string HttpParser::getRequestData(char* bufferPtr)
{
	std::string firstLine("");
	while (*bufferPtr != '\r')	//extract the first line from buffer
	{
		firstLine += *bufferPtr;
		bufferPtr++;
	}
	return firstLine;
}

std::string HttpParser::parseRequestData(std::string toParse)
{
	std::string extractedSubmatchPath("");
	std::smatch pieces_match;
	if (std::regex_match(toParse, pieces_match, std::regex(REGEX::REQUEST_REGEX)))
	{
		extractedSubmatchPath = pieces_match[2].str();
	}
	return extractedSubmatchPath;	//if there is no match so the request is not HTTP it will return empty string
}
