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
	class RequestPost : private RequestData
	{
		friend class Request;
	private:
		std::string body;
	public:
		std::string getBody();
	};
public:
	RequestData HEADERS;
	RequestData DATA;
	RequestData GET;
	RequestPost POST;
	RequestData COOKIE;
	Request(const std::string request, const std::string method, const std::string url);

	class Parser
	{
	public:
		static Request parseRequestData(char* toParse, std::mutex& lock);
		static REQUEST_METHOD getRequestMethod(const std::string method);
		static std::string parseUrl(const std::string url, std::map<std::string, std::string>& container);
		static std::string parseVal(const std::string value);
		static void parseCookies(const std::string cookies, std::map<std::string, std::string>& container);
		static void parseHeaders(const std::string request, std::map<std::string, std::string>& headers, std::map<std::string, std::string>& cookies);
		static CONTENT_TYPE getContentType(const std::string contentTypeStr);
		static std::string getBody(const std::string request);
		static std::string getHeaders(const std::string request);
		static void parseBody(Request& request);
		static void parseFormUrlEncoded(Request& request);
		static char codeToSymbol(const std::string str);
		static void percentDecode(std::string& str);
	};
};
