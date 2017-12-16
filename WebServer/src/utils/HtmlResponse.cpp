#include "../include/HtmlResponse.h"
#include <iostream>

std::string HTMLResponse::HttpResponse(const std::string html, const std::string status, const size_t statusCode)
{
	std::string response("HTTP/1.0 " + std::to_string(statusCode) + " " + status + " \r\n");
	std::cout << "Client response: " << response;
	response += "Content-Type: text/html \r\n";
	response += html;
	return response;
}
std::string HTMLResponse::errorPage(const size_t code, const std::string msg)
{
	std::string response("HTTP/1.0 " + std::to_string(code) + " " + msg + " \r\n");
	std::cout << "Client response: " << response;
	response += "Content-Type: text/html \r\n";
	response += "<!DOCTYPE html>\n<html>\n\n<head>\n<title>" + 
		msg + "</title>\n</head>\n\n<body>\n<h1 style=\"text-align: center;\">" +
		std::to_string(code) + " " + msg + "</h1>\n</body>\n\n</html>\n";
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
