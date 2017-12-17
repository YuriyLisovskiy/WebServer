#include "../include/HttpResponse.h"
#include "../include/Header.h"
#include <fstream>

std::string Response::HttpResponse(const std::string filePath, const std::string status, const size_t statusCode)
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
	return response;
}
std::string Response::errorPage(const size_t code, const std::string msg)
{
	std::string response("HTTP/1.0 " + std::to_string(code) + " " + msg + " \r\n");
	response += "Content-Type: text/html \r\n";
	response += "<!DOCTYPE html>\n<html>\n\n<head>\n<title>" + 
		msg + "</title>\n</head>\n\n<body>\n<h1 style=\"text-align: center;\">" +
		std::to_string(code) + " " + msg + "</h1>\n</body>\n\n</html>\n";
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
