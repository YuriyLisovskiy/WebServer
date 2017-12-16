#include "../include/HtmlResponse.h"
#include <iostream>
#include <fstream>

std::string HTMLResponse::HttpResponse(const std::string filePath, const std::string status, const size_t statusCode)
{
	std::ifstream file(filePath);
	std::string html("");
	if (file.is_open())
	{
		html.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();
	}
	std::string response("HTTP/1.0 " + std::to_string(statusCode) + " " + status + " \r\n");
	response += "Content-Type: text/html \r\n";
	response += html;
	std::cout << "HTTP/1.0 " + std::to_string(statusCode) + " " + status + "\n";
	return response;
}
std::string HTMLResponse::errorPage(const size_t code, const std::string msg)
{
	std::string response("HTTP/1.0 " + std::to_string(code) + " " + msg + " \r\n");
	response += "Content-Type: text/html \r\n";
	response += "<!DOCTYPE html>\n<html>\n\n<head>\n<title>" + 
		msg + "</title>\n</head>\n\n<body>\n<h1 style=\"text-align: center;\">" +
		std::to_string(code) + " " + msg + "</h1>\n</body>\n\n</html>\n";
	std::cout << "HTTP/1.0 " + std::to_string(code) + " " + msg + "\n";
	return response;
}
std::string HTMLResponse::NotFound()
{
	return HTMLResponse::errorPage(404, "Not Found");
}
std::string HTMLResponse::Forbidden()
{
	return HTMLResponse::errorPage(403, "Forbidden");;
}
std::string HTMLResponse::MethodNotAllowed()
{
	return HTMLResponse::errorPage(405, "Method Not Allowed");;
}
std::string HTMLResponse::InternalServerError()
{
	return HTMLResponse::errorPage(500, "Internal Server Error");
}
