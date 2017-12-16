#include "HtmlResponse.h"
#include <iostream>

std::string HTMLResponse::ok(const std::string html)
{
	std::string response("HTTP/1.0 200 OK \r\n");
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
std::string HTMLResponse::notFound()
{
	return HTMLResponse::errorPage(404, "Not Found");
}
std::string HTMLResponse::forbidden()
{
	return HTMLResponse::errorPage(403, "Forbidden");;
}
std::string HTMLResponse::methodNotAllowed()
{
	return HTMLResponse::errorPage(405, "Method Not Allowed");;
}
std::string HTMLResponse::internalServerError()
{
	return HTMLResponse::errorPage(500, "Internal Server Error");
}
