#include "../include/HttpServer.h"
#include <regex>
#include <iostream>

std::string HTTP::HttpServer::Parser::getClientData(SOCK client, int port, int clientID)
{
	std::string result = "ID: " + std::to_string(clientID) + "\nThe Client port is: ";
	result += std::to_string(port);
	result += "\nThe Client IP is: ";
	result += Parser::getIP(client);
	result += '\n';
	return result;
}

BaseView* HTTP::HttpServer::Parser::urlIsAvailable(std::vector<BaseView*> views, const std::string url)
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

std::string HTTP::HttpServer::Parser::getIP(SOCK socket)
{
	struct sockaddr_in addr;
	socklen_t addr_size = sizeof(struct sockaddr_in);
	getsockname(socket, (struct sockaddr *)&addr, &addr_size);
	sockaddr_in* pV4Addr = (struct sockaddr_in*)&addr;
	int ipAddr = pV4Addr->sin_addr.s_addr;
	char clientIp[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &ipAddr, clientIp, INET_ADDRSTRLEN);
	return clientIp;
}
