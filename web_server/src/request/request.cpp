#include "../include/server.h"

http::Request::Request(const std::string& request, const std::string& method, const std::string& url, const std::string& client)
{
	std::string headers;
	this->DATA.dict["method"] = method;
	this->DATA.dict["client"] = client;
	Parser::parseHeaders(Parser::getHeaders(request), this->HEADERS.dict, this->COOKIE.dict);
	if (method == "GET")
	{
		this->DATA.dict["url"] = Parser::parseUrl(url, this->GET.dict);
	}
	else
	{
		this->DATA.dict["url"] = url;
		this->POST.body = Parser::getBody(request);
		Parser::parseBody(*this);
	}
};
std::string http::Request::RequestData::get(const std::string& key)
{
	std::string result;
	if (this->dict.find(key) != this->dict.end())
	{
		result = this->dict[key];
	}
	return result;
}
std::string http::Request::RequestPost::getBody()
{
	return this->body;
}
