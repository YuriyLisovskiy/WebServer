#include "../include/HttpResponse.h"
#include <fstream>
#include <iostream>
#include <regex>

std::string Response::Parser::errorPage(const size_t code, const std::string msg)
{
	std::string html("<!DOCTYPE html>\n<html>\n<head>\n"
		"<link rel=\"icon\" "
		"href=\"http://www.freeiconspng.com/uploads/delete-error-exit-remove-stop-x-cross-icon--28.png\" "
		"type=\"image/png\"><title>" +
		msg + "</title>\n</head>\n<body>\n<h1 style=\"text-align: center;\">" +
		std::to_string(code) + " " + msg + "</h1>\n</body>\n</html>\n");
	return Parser::makeResponse(html, msg, code);
}

std::string Response::Parser::makeResponse(const std::string html, const std::string statusStr, const size_t statusCode)
{
	std::string response("HTTP/1.1 " + std::to_string(statusCode) + " " + statusStr + " \r\n");
	response += "Content-Type: text/html; charset=utf-8 \r\n";
	response += "Content-Length: " + std::to_string(html.length()) + " \r\n\n";
	response += html;
	std::cout << statusCode << '\n';
	return response;
}

std::string Response::Parser::parseTemplate(const std::string html, std::map<std::string, std::string> context)
{
	std::string result("");
	if (!html.empty())
	{
		std::string copy(html);
		std::regex exp(R"(\{\s*\{\s*(\w+)\s*\}\s*\})");
		std::smatch data;
		while (std::regex_search(copy, data, exp))
		{
			result += data.prefix();
			result += context[data[1].str()];
			copy = data.suffix();
		}
		result += std::string(html.begin() + html.find_last_of("}") + 2, html.end());
	}
	return result;
}

std::string Response::Parser::readFile(const std::string filePath)
{
	std::ifstream file(filePath);
	std::string html("");
	if (file.is_open())
	{
		html.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();
	}
	return html;
}
