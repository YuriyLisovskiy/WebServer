#pragma once
#include <string>
#include <map>

class Request
{
private:
	class RequestData
	{
		friend class Request;
	private:
		std::map<std::string, std::string> dict;
	public:
		std::string get(const std::string key);
	};
public:
	RequestData HEADERS;
	RequestData DATA;
	RequestData GET;
	RequestData POST;
	RequestData COOKIES;
	Request(const std::string request, const std::string method, const std::string url);
};
