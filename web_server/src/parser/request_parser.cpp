#include "../include/request.h"
#include "../include/regexpr.h"
#include <regex>
#include <iostream>

http::Request http::Request::Parser::parseRequestData(char* toParse, std::mutex& lock, const std::string client)
{
	std::string firstLine;
	if (toParse)
	{
		while (*toParse != '\r')
		{
			firstLine += *toParse;
			toParse++;
		}
	}
	else
	{
		throw "bad";
	}
	toParse += 2;
	lock.lock();
	std::cout << "\n[";
	DATE_TIME_NOW(std::cout, "%d/%b/%Y %r");
	std::cout << "] \"" << firstLine << "\" ";
	lock.unlock();
	std::string url, method;
	std::cmatch data;
	if (std::regex_match(firstLine.c_str(), data, std::regex(regex::FIRST_LINE_REQUEST)))
	{
		method = data[1].str();
		url = data[2].str();
	}
	else
	{
		throw "bad request";
	}
	std::string body(toParse);
	body = std::regex_replace(body, std::regex(R"(\r+)"), "");
	return Request(body, method, url, client);
}
REQUEST_METHOD http::Request::Parser::getRequestMethod(const std::string method)
{
	REQUEST_METHOD result = REQUEST_METHOD::REQUEST_NONE;
	std::string methodToLower(method);
	std::transform(methodToLower.begin(), methodToLower.end(), methodToLower.begin(), ::tolower);
	if (methodToLower == "get")
	{
		result = REQUEST_METHOD::GET;
	}
	else if (methodToLower == "post")
	{
		result = REQUEST_METHOD::POST;
	}
	else if (methodToLower == "put")
	{
		result = REQUEST_METHOD::PUT;
	}
	else if (methodToLower == "delete")
	{
		result = REQUEST_METHOD::DElETE;
	}
	else if (methodToLower == "options")
	{
		result = REQUEST_METHOD::OPTIONS;
	}
	else if (methodToLower == "connect")
	{
		result = REQUEST_METHOD::CONNECT;
	}
	else if (methodToLower == "head")
	{
		result = REQUEST_METHOD::HEAD;
	}
	else if (methodToLower == "trace")
	{
		result = REQUEST_METHOD::TRACE;
	}
	else if (methodToLower == "patch")
	{
		result = REQUEST_METHOD::PATCH;
	}
	return result;
}
std::string http::Request::Parser::parseUrl(const std::string url, std::map<std::string, std::string>& container)
{
	std::string result(url);
	size_t posBegin = url.find('?');
	if (posBegin != std::string::npos)
	{
		result = std::string(url.begin(), url.begin() + posBegin);
		size_t posEnd = url.find('&');
		std::string line;
		std::cmatch data;
		while (posEnd != std::string::npos)
		{
			line = std::string(url.begin() + posBegin + 1, url.begin() + posEnd);
			if (std::regex_match(line.c_str(), data, std::regex(regex::REQUEST_GET_PARAM)))
			{
				container[data[1].str()] = Parser::parseVal(data[2].str());
			}
			posBegin = posEnd;
			posEnd = url.find('&', posBegin + 1);
		}
		posBegin++;
		if (posBegin < url.size())
		{
			line = std::string(url.begin() + posBegin, url.end());
			if (std::regex_match(line.c_str(), data, std::regex(regex::REQUEST_GET_PARAM)))
			{
				container[data[1].str()] = Parser::parseVal(data[2].str());
			}
		}
	}
	while (!result.empty() && result[result.size() - 1] == '/')
	{
		result.pop_back();
	}
	return result;
}
std::string http::Request::Parser::parseVal(const std::string value)
{
	return std::regex_replace(value, std::regex(R"(\+)"), " ");
}
void http::Request::Parser::parseCookies(const std::string cookies, std::map<std::string, std::string>& container)
{
	if (!cookies.empty())
	{
		size_t posBegin = 1, posEnd = cookies.find('=');
		std::string key, value;
		while (posEnd != std::string::npos)
		{
			key = std::string(cookies.begin() + posBegin, cookies.begin() + posEnd);
			std::transform(key.begin(), key.end(), key.begin(), ::tolower);

			posBegin = posEnd + 1;
			posEnd = cookies.find(';', posBegin);
			if (posEnd == std::string::npos)
			{
				value = std::string(cookies.begin() + posBegin, cookies.end());
			}
			else
			{
				value = std::string(cookies.begin() + posBegin, cookies.begin() + posEnd);
			}
			posBegin = posEnd + 2;
			posEnd = cookies.find('=', posEnd);
			container[key] = std::regex_replace(value, std::regex(R"(^\s+)"), "");
		}
	}
}
void http::Request::Parser::parseHeaders(const std::string request, std::map<std::string, std::string>& headers, std::map<std::string, std::string>& cookies)
{
	if (!request.empty())
	{
		size_t posBegin = 0, posEnd = request.find(':');
		std::string key, value;
		do
		{
			key = std::string(request.begin() + posBegin, request.begin() + posEnd);
			std::transform(key.begin(), key.end(), key.begin(), ::tolower);
			if (key == "cookie")
			{
				posBegin = posEnd + 1;
				posEnd = request.find('\n', posBegin);
				std::string cookie;
				if (posEnd == std::string::npos)
				{
					cookie = std::string(request.begin() + posBegin, request.end());
				}
				else
				{
					cookie = std::string(request.begin() + posBegin, request.begin() + posEnd);
				}
				Parser::parseCookies(cookie, cookies);
			}
			else
			{
				posBegin = posEnd + 1;
				posEnd = request.find('\n', posBegin);
				if (posEnd == std::string::npos)
				{
					value = std::string(request.begin() + posBegin, request.end());
				}
				else
				{
					value = std::string(request.begin() + posBegin, request.begin() + posEnd);
				}
				posBegin = posEnd + 1;
				posEnd = request.find(':', posEnd);
				headers[key] = std::regex_replace(value, std::regex(R"(^\s+)"), "");
			}
		} while (posEnd != std::string::npos);
	}
}
CONTENT_TYPE http::Request::Parser::getContentType(const std::string contentTypeStr)
{
	CONTENT_TYPE type = CONTENT_TYPE::CONTENT_NONE;
	if (contentTypeStr == X_WWW_FORM_URLENCODED_TYPE)
	{
		type = CONTENT_TYPE::X_WWW_FORM_URLENCODED;
	}
	else if (contentTypeStr == JSON_TYPE)
	{
		type = CONTENT_TYPE::JSON;
	}
	return type;
}
std::string http::Request::Parser::getBody(const std::string request)
{
	std::string requestBody;
	if (!request.empty())
	{
		size_t pos = request.find("\n\n");
		if (pos != std::string::npos)
		{
			requestBody = std::string(request.begin() + pos + 2, request.end());
		}
	}
	return requestBody;
}
std::string http::Request::Parser::getHeaders(const std::string request)
{
	std::string headers;
	if (!request.empty())
	{
		size_t pos = request.find("\n\n");
		if (pos != std::string::npos)
		{
			headers = std::string(request.begin(), request.begin() + pos);
		}
	}
	return headers;
}
void http::Request::Parser::parseBody(Request& request)
{
	switch (Parser::getContentType(request.HEADERS.get("content-type")))
	{
	case CONTENT_TYPE::X_WWW_FORM_URLENCODED:
		Parser::parseFormUrlEncoded(request);
		break;
	case CONTENT_TYPE::JSON:
		request.POST.body = request.HEADERS.get("content-type") + '\n' + request.POST.body;
		break;
	default:
		throw "invalid content type";
	}
}
void http::Request::Parser::parseFormUrlEncoded(Request& request)
{
	Parser::percentDecode(request.POST.body);
	std::string form(request.POST.body);
	size_t posBegin = 0, posEnd = form.find('&');
	std::string line;
	std::cmatch data;
	while (posEnd != std::string::npos)
	{
		line = std::string(form.begin() + posBegin, form.begin() + posEnd);
		if (std::regex_match(line.c_str(), data, std::regex(regex::REQUEST_GET_PARAM)))
		{
			request.POST.dict[data[1].str()] = Parser::parseVal(data[2].str());
		}
		posBegin = posEnd + 1;
		posEnd = form.find('&', posBegin + 1);
	}
	if (posBegin < form.size())
	{
		line = std::string(form.begin() + posBegin, form.end());
		if (std::regex_match(line.c_str(), data, std::regex(regex::REQUEST_GET_PARAM)))
		{
			request.POST.dict[data[1].str()] = Parser::parseVal(data[2].str());
		}
	}
}
char http::Request::Parser::codeToSymbol(const std::string str)
{
	if (str == "%20")
	{
		return ' ';
	}
	else if (str == "%21")
	{
		return '!';
	}
	else if (str == "%23")
	{
		return '#';
	}
	else if (str == "%24")
	{
		return '$';
	}
	else if (str == "%26")
	{
		return '&';
	}
	else if (str == "%27")
	{
		return '\'';
	}
	else if (str == "%28")
	{
		return '(';
	}
	else if (str == "%29")
	{
		return ')';
	}
	else if (str == "%2A")
	{
		return '*';
	}
	else if (str == "%2B")
	{
		return '+';
	}
	else if (str == "%2C")
	{
		return ',';
	}
	else if (str == "%2F")
	{
		return '/';
	}
	else if (str == "%3A")
	{
		return ':';
	}
	else if (str == "%3B")
	{
		return ';';
	}
	else if (str == "%3D")
	{
		return '=';
	}
	else if (str == "%3F")
	{
		return '?';
	}
	else if (str == "%40")
	{
		return '@';
	}
	else if (str == "%5B")
	{
		return '[';
	}
	else if (str == " %5D")
	{
		return ']';
	}
	else
	{
		return '?';
	}
}
void http::Request::Parser::percentDecode(std::string& str)
{
	size_t pos = str.find('%');
	while (pos != std::string::npos)
	{
		str.insert(str.begin() + pos, 1, Parser::codeToSymbol(std::string(str.begin() + pos, str.begin() + pos + 3)));
		str.erase(str.begin() + pos + 1, str.begin() + pos + 4);
		pos = str.find('%');
	}
}
