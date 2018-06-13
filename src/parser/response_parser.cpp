#include "../include/response.h"
#include <iostream>
#include <sstream>
#include <fstream>

std::string http::Response::Parser::errorPage(const size_t code, const std::string& msg)
{
	std::string html("<!DOCTYPE html>\n<html>\n<head>\n"
		"<link rel=\"icon\" "
		"href=\"http://www.freeiconspng.com/uploads/delete-error-exit-remove-stop-x-cross-icon--28.png\" "
		"type=\"image/png\"><title>" +
		msg + "</title>\n</head>\n<body>\n<h1 style=\"text-align: center;\">" +
		std::to_string(code) + " " + msg + "</h1>\n</body>\n</html>\n");
	return Parser::makeResponse(html, msg, code, "html");
}
std::string http::Response::Parser::readFile(const std::string& filePath, bool openBinary)
{
	std::ifstream file;
	if (openBinary)
	{
		file.open(filePath, std::ios::binary);
	}
	else
	{
		file.open(filePath);
	}
	std::string res;
	if (file.is_open())
	{
		res.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();
	}
	else
	{
		std::cerr << "Cannot open '" << filePath << "'\n";
	}
	return res;
}
std::string http::Response::Parser::makeResponse(const std::string& html, const std::string& statusStr, const size_t statusCode, const std::string& requestContent)
{
	std::ostringstream ss;
	DATE_TIME_NOW(ss, "%a, %d %b %y %T %z");
	std::string response("HTTP/1.1 " + std::to_string(statusCode) + " " + statusStr + " \r\n");
	response += "Content-Type: " + Parser::setContentType(requestContent) + "; charset=utf-8 \r\n";
	response += "Content-Length: " + std::to_string(html.length()) + " \r\n";
	response += "Date: " + ss.str() + " \r\n\r\n";
	response += html;
	std::cout << statusCode << '\n';
	return response;
}
std::string http::Response::Parser::setContentType(const std::string& requestType)
{
	std::string res("text/");
	if (requestType == "js")
	{
		res.append("javascript");
	}
	else if (IMAGE_TYPE(requestType))
	{
		res = "image/*";
	}
	else if (AUDIO_TYPE(requestType))
	{
		res = "audio/*";
	}
	else
	{
		res.append(requestType);
	}
	return res;
}
