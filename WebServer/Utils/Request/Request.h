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
	class RequestGET
	{
		friend class Request;
	private:
		std::map<std::string, std::string> dict;
	public:
		std::string get(const std::string key);
	};
	class RequestPOST
	{
		friend class Request;
	private:
		std::map<std::string, std::string> dict;
	public:
		std::string get(const std::string key);
	};
	std::string parseUrl(const std::string url);
	std::string parseValue(const std::string value);
public:
	RequestData DATA;
	RequestGET GET;
	RequestPOST POST;
	Request(const std::string request, const std::string method, const std::string url);
};
