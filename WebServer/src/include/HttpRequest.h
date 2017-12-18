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
	std::string parseUrl(const std::string url);
	std::string parseValue(const std::string value);
	void parseCookies(const std::string cookies);
	void parseHeaders(const std::string headers);
public:
	RequestData HEADERS;
	RequestData DATA;
	RequestData GET;
	RequestData POST;
	RequestData COOKIES;
	Request(const std::string request, const std::string method, const std::string url);
};
