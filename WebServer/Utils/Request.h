#pragma once
#include <string>
#include <map>

class Request
{
private:
	std::map<std::string, std::string> dict;
public:
	Request(const std::string request, const std::string method, const std::string url);
	std::string get(const std::string key);
};
