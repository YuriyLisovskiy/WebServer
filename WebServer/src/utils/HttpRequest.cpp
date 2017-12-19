#include "../include/HttpRequest.h"
#include "../include/Parser.h"

Request::Request(const std::string body, const std::string method, const std::string url)
{
	size_t pos = body.rfind("\n\n");
	std::string headers("");
	headers = std::string(body.begin(), body.begin() + pos);
	this->DATA.dict["headers"] = headers;
	this->DATA.dict["method"] = method;
	if (method == "GET")
	{
		this->DATA.dict["url"] = Parser::parseUrl(url, this->GET.dict);
	}
	else
	{
		this->DATA.dict["url"] = url;
		// TODO: parse form input.
	}
	Parser::parseHeaders(headers, this->HEADERS.dict, this->COOKIES.dict);
};

std::string Request::RequestData::get(const std::string key)
{
	std::string result("");
	if (this->dict.find(key) != this->dict.end())
	{
		result = this->dict[key];
	}
	return result;
}
