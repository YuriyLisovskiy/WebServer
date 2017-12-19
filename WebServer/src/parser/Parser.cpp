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
	std::cout << '\n';
	DATE_TIME_NOW(std::cout);
	std::cout << " \"" << firstLine << "\" ";
	lock.unlock();
	std::string url(""), method("");
	std::smatch data;
	if (std::regex_match(firstLine, data, std::regex(REGEX::FIRST_LINE_REQUEST)))
	{
		method = data[1].str();
		url = data[2].str();
	}
	else
	{
		throw std::exception("bad request");
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

std::string Parser::parseUrl(const std::string url, std::map<std::string, std::string>& container)
{
	std::string result(url);
	size_t posBegin = url.find('?');
	if (posBegin != std::string::npos)
	{
		result = std::string(url.begin(), url.begin() + posBegin);
		size_t posEnd = url.find('&');
		std::string line("");
		std::smatch data;
		while (posEnd != std::string::npos)
		{
			line = std::string(url.begin() + posBegin + 1, url.begin() + posEnd);
			if (std::regex_match(line, data, std::regex(REGEX::REQUEST_GET_PARAM)))
			{
				container[data[1].str()] = Parser::parseValue(data[2].str());
			}
			posBegin = posEnd;
			posEnd = url.find('&', posBegin + 1);
		}
		posBegin++;
		if (posBegin < url.size())
		{
			line = std::string(url.begin() + posBegin, url.end());
			if (std::regex_match(line, data, std::regex(REGEX::REQUEST_GET_PARAM)))
			{
				container[data[1].str()] = Parser::parseValue(data[2].str());
			}
		}
	}
	while (result[result.size() - 1] == '/')
	{
		result.pop_back();
	}
	return result;
}

std::string Parser::parseValue(const std::string value)
{
	return std::regex_replace(value, std::regex("\\+"), " ");
}

void Parser::parseCookies(const std::string cookies, std::map<std::string, std::string>& container)
{
	if (!cookies.empty())
	{
		size_t posBegin = 1, posEnd = cookies.find('=');
		std::string key(""), value("");
		while (posEnd != std::string::npos)
		{
			key = std::string(cookies.begin() + posBegin, cookies.begin() + posEnd);
			std::transform(key.begin(), key.end(), key.begin(), ::tolower);

			posBegin = posEnd + 1;
			posEnd = cookies.find(';', posBegin);
			if (posEnd == std::string::npos)
			{
				value = std::string(cookies.begin() + posBegin, cookies.end());
			}
			else
			{
				value = std::string(cookies.begin() + posBegin, cookies.begin() + posEnd);
			}
			posBegin = posEnd + 2;
			posEnd = cookies.find('=', posEnd);
			container[key] = std::regex_replace(value, std::regex("^\\s+"), "");
		}
	}
}

void Parser::parseHeaders(const std::string headers, std::map<std::string, std::string>& container, std::map<std::string, std::string>& cookiesContainer)
{
	if (!headers.empty())
	{
		size_t posBegin = 0, posEnd = headers.find(':');
		std::string key(""), value("");
		do
		{
			key = std::string(headers.begin() + posBegin, headers.begin() + posEnd);
			std::transform(key.begin(), key.end(), key.begin(), ::tolower);
			if (key == "cookie")
			{
				posBegin = posEnd + 1;
				posEnd = headers.find('\n', posBegin);
				std::string cookies("");
				if (posEnd == std::string::npos)
				{
					cookies = std::string(headers.begin() + posBegin, headers.end());
				}
				else
				{
					cookies = std::string(headers.begin() + posBegin, headers.begin() + posEnd);
				}
				Parser::parseCookies(cookies, cookiesContainer);
			}
			else
			{
				posBegin = posEnd + 1;
				posEnd = headers.find('\n', posBegin);
				if (posEnd == std::string::npos)
				{
					value = std::string(headers.begin() + posBegin, headers.end());
				}
				else
				{
					value = std::string(headers.begin() + posBegin, headers.begin() + posEnd);
				}
				posBegin = posEnd + 1;
				posEnd = headers.find(':', posEnd);
				container[key] = std::regex_replace(value, std::regex("^\\s+"), "");
			}
		} while (posEnd != std::string::npos);
	}
}
