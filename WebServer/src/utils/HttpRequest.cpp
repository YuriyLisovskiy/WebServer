#include "../include/HttpRequest.h"
#include "../include/RegularExpressions.h"
#include <algorithm>
#include <regex>

Request::Request(const std::string body, const std::string method, const std::string url)
{
	size_t pos = body.rfind("\n\n");
	std::string headers("");
	headers = std::string(body.begin(), body.begin() + pos);
	this->DATA.dict["headers"] = headers;
	this->DATA.dict["method"] = method;
	if (method == "GET")
	{
		this->DATA.dict["url"] = Request::parseUrl(url);
	}
	else
	{
		this->DATA.dict["url"] = url;
		// TODO: parse form input.
	}
	this->parseHeaders(headers);
};

std::string Request::parseUrl(const std::string url)
{
	std::string result(url);
	size_t posBegin = url.find('?');
	if (posBegin != std::string::npos)
	{
		result = std::string(url.begin(), url.begin() + posBegin);
		size_t posEnd = url.find('&');
		std::string line("");
		std::smatch data;
		while (posEnd != std::string::npos)
		{
			line = std::string(url.begin() + posBegin + 1, url.begin() + posEnd);
			if (std::regex_match(line, data, std::regex(REGEX::REQUEST_GET_PARAM)))
			{
				this->GET.dict[data[1].str()] = Request::parseValue(data[2].str());
			}
			posBegin = posEnd;
			posEnd = url.find('&', posBegin + 1);
		}
		posBegin++;
		if (posBegin < url.size())
		{
			line = std::string(url.begin() + posBegin, url.end());
			if (std::regex_match(line, data, std::regex(REGEX::REQUEST_GET_PARAM)))
			{
				this->GET.dict[data[1].str()] = Request::parseValue(data[2].str());
			}
		}
	}
	while (result[result.size() - 1] == '/')
	{
		result.pop_back();
	}
	return result;
}

std::string Request::parseValue(const std::string value)
{
	return std::regex_replace(value, std::regex("\\+"), " ");
}

void Request::parseCookies(const std::string cookies)
{
	if (!cookies.empty())
	{
		size_t posBegin = 1, posEnd = cookies.find('=');
		std::string key(""), value("");
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
			this->COOKIES.dict[key] = std::regex_replace(value, std::regex("^\\s+"), "");
		}
	}
}

void Request::parseHeaders(const std::string headers)
{
	if (!headers.empty())
	{
		size_t posBegin = 0, posEnd = headers.find(':');
		std::string key(""), value("");
		do
		{
			key = std::string(headers.begin() + posBegin, headers.begin() + posEnd);
			std::transform(key.begin(), key.end(), key.begin(), ::tolower);
			if (key == "cookie")
			{
				posBegin = posEnd + 1;
				posEnd = headers.find('\n', posBegin);
				std::string cookies("");
				if (posEnd == std::string::npos)
				{
					cookies = std::string(headers.begin() + posBegin, headers.end());
				}
				else
				{
					cookies = std::string(headers.begin() + posBegin, headers.begin() + posEnd);
				}
				this->parseCookies(cookies);
			}
			else
			{
				posBegin = posEnd + 1;
				posEnd = headers.find('\n', posBegin);
				if (posEnd == std::string::npos)
				{
					value = std::string(headers.begin() + posBegin, headers.end());
				}
				else
				{
					value = std::string(headers.begin() + posBegin, headers.begin() + posEnd);
				}
				posBegin = posEnd + 1;
				posEnd = headers.find(':', posEnd);
				this->HEADERS.dict[key] = std::regex_replace(value, std::regex("^\\s+"), "");
			}
		} while (posEnd != std::string::npos);
	}
}

std::string Request::RequestData::get(const std::string key)
{
	std::string result("");
	if (this->dict.find(key) != this->dict.end())
	{
		result = this->dict[key];
	}
	return result;
}
