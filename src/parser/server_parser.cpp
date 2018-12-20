#include <regex>
#include <iostream>

#include "../include/server.h"

std::string http::Server::Parser::getClientData(const SOCK& client, uint16_t port, int clientID)
{
	return "ID: " + std::to_string(clientID) + "\nThe Client port is: " + std::to_string(port) + "\nThe Client IP is: " + Parser::getIP(client) + '\n';
}

std::string http::Server::Parser::getIP(const SOCK& socket)
{
	struct sockaddr_in addr = {};
	socklen_t addr_size = sizeof(struct sockaddr_in);
	getsockname(socket, (struct sockaddr *)&addr, &addr_size);
	auto* pV4Addr = &addr;
	int ipAddr = pV4Addr->sin_addr.s_addr;
	char clientIp[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &ipAddr, clientIp, INET_ADDRSTRLEN);
	return std::string(clientIp);
}

bool http::Server::Parser::requestStatic(const std::string& url)
{
	return url.find('.') != std::string::npos;
}
