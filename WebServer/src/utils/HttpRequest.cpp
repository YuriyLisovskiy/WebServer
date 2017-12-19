#include "../include/HttpRequest.h"

#include <iostream>

Request::Request(const std::string request, const std::string method, const std::string url)
{
	std::string headers("");
	this->DATA.dict["method"] = method;
	Request::Parser::parseHeaders(Request::Parser::getHeaders(request), this->HEADERS.dict, this->COOKIE.dict);
	if (method == "GET")
	{
		this->DATA.dict["url"] = Request::Parser::parseUrl(url, this->GET.dict);
	}
	else
	{
		this->DATA.dict["url"] = url;
		this->POST.body = Request::Parser::getBody(request);
		Request::Parser::parseBody(*this);
	}
	for (auto& var : this->POST.dict)
	{
		std::cout << var.first << " - " << var.second << "\n";
	}
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

std::string Request::RequestPost::getBody()
{
	return this->body;
}
