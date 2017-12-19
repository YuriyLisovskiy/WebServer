#pragma once
#include "RequestMacros.h"
#include <string>
#include <map>
#include <mutex>

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

	class Parser
	{
	public:
		static Request parseRequestData(char* toParse, std::mutex& lock);
		static rMethod getRequestMethod(const std::string method);
		static std::string parseUrl(const std::string url, std::map<std::string, std::string>& container);
		static std::string parseValue(const std::string value);
		static void parseCookies(const std::string cookies, std::map<std::string, std::string>& container);
		static void parseHeaders(const std::string headers, std::map<std::string, std::string>& container, std::map<std::string, std::string>& cookiesContainer);
		static contentType getContentType(const std::string contentTypeStr);
	};
};
