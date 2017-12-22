#include "../include/HttpResponse.h"
#include "../include/ServerMacros.h"
#include <fstream>
#include <iostream>
#include <regex>

std::string Response::HttpResponse(const std::string filePath, const std::string status, const size_t statusCode)
{
	std::ifstream file(filePath);
	std::string html("");
	if (file.is_open())
	{
		html.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();
	}
	std::string response("HTTP/1.1 " + std::to_string(statusCode) + " " + status + " \r\n");
	response += "Content-Type: text/html; charset=utf-8 \r\n";
	response += "Content-Length: " + std::to_string(html.length()) + " \r\n\n";
	response += html;
	std::cout << statusCode << '\n';
	return response;
}
std::string Response::errorPage(const size_t code, const std::string msg)
{
	std::string html("<!DOCTYPE html>\n<html>\n<head>\n"
		"<link rel=\"icon\" "
		"href=\"http://www.freeiconspng.com/uploads/delete-error-exit-remove-stop-x-cross-icon--28.png\" "
		"type=\"image/png\"><title>" +
		msg + "</title>\n</head>\n<body>\n<h1 style=\"text-align: center;\">" +
		std::to_string(code) + " " + msg + "</h1>\n</body>\n</html>\n");
	std::string response("HTTP/1.0 " + std::to_string(code) + " " + msg + " \r\n");
	response += "Content-Type: text/html\r\n";
	response += "Content-Length: " + std::to_string(html.length()) + " \r\n\n";
	response += html;
	std::cout << code << '\n';
	return response;
}
std::string Response::NotFound()
{
	return Response::errorPage(404, "Not Found");
}
std::string Response::Forbidden()
{
	return Response::errorPage(403, "Forbidden");;
}
std::string Response::MethodNotAllowed()
{
	return Response::errorPage(405, "Method Not Allowed");;
}
std::string Response::InternalServerError()
{
	return Response::errorPage(500, "Internal Server Error");
}
std::string Response::BadRequest()
{
	return Response::errorPage(400, "Bad Request");
}
std::string Response::render(const std::string filePath, std::map<std::string, std::string> context, const std::string status, const size_t statusCode)
{
	std::ifstream file(filePath);
	std::string html("");
	if (file.is_open())
	{
		html.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();
	}
	html = Parser::parseTemplate(html, context);
	std::string response("HTTP/1.1 " + std::to_string(statusCode) + " " + status + " \r\n");
	response += "Content-Type: text/html; charset=utf-8 \r\n";
	response += "Content-Length: " + std::to_string(html.length()) + " \r\n\n";
	response += html;
	std::cout << statusCode << '\n';
	return response;
}
std::string Response::Parser::parseTemplate(const std::string html, std::map<std::string, std::string> context)
{
	std::string result(""), copy(html);
	std::regex exp(R"(\{\s*\{\s*(\w+)\s*\}\s*\})");
	std::smatch data;
	while (std::regex_search(copy, data, exp))
	{
		result += data.prefix();
		result += context[data[1].str()];
		copy = data.suffix();
	}
	result += std::string(html.begin() + html.find_last_of('}') + 1, html.end());
	return result;
}
