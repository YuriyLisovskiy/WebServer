#include "Request.h"
#include <algorithm>
#include <regex>

Request::Request(const std::string body, const std::string method, const std::string url)
{
	this->dict["body"] = body;
	this->dict["method"] = method;
	this->dict["url"] = url;
	if (body.size() > 0)
	{
		size_t posBegin = 0, posEnd = body.find(':');
		std::string key(""), value("");
		do
		{
			key = std::string(body.begin() + posBegin, body.begin() + posEnd);
			std::transform(key.begin(), key.end(), key.begin(), ::tolower);
			if (key == "cookie")
			{
				key = "csrftoken";
				posBegin = body.find('=', posEnd) + 1;
			}
			else
			{
				posBegin = posEnd + 1;
			}
			posEnd = body.find('\n', posBegin);
			value = std::string(body.begin() + posBegin, body.begin() + posEnd);
			posBegin = posEnd + 1;
			posEnd = body.find(':', posBegin);
			this->dict[key] = std::regex_replace(value, std::regex("^\\s+"), "");
		} while (posEnd != std::string::npos);
	}
};

std::string Request::get(const std::string key)
{
	std::string result("");
	if (this->dict.find(key) != this->dict.end())
	{
		result = this->dict[key];
	}
	return result;
}
