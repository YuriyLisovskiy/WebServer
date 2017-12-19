#include "../include/HttpRequest.h"

#include <iostream>

Request::Request(const std::string body, const std::string method, const std::string url)
{
	std::cout << body;
	std::string headers("");
	this->DATA.dict["headers"] = body;
	this->DATA.dict["method"] = method;
	if (method == "GET")
	{
		this->DATA.dict["url"] = Request::Parser::parseUrl(url, this->GET.dict);
	}
	else
	{
		this->DATA.dict["url"] = url;
		// TODO: parse form input.
	}
	Request::Parser::parseHeaders(body, this->HEADERS.dict, this->COOKIES.dict);
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
