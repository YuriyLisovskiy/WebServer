#include "../include/HttpResponse.h"

std::string Response::HttpResponse(const std::string filePath, const std::string status, const size_t statusCode)
{
	return Parser::makeResponse(Parser::readFile(filePath), status, statusCode, "html");
}
std::string Response::NotFound()
{
	return Parser::errorPage(404, "Not Found");
}
std::string Response::Forbidden()
{
	return Parser::errorPage(403, "Forbidden");;
}
std::string Response::MethodNotAllowed()
{
	return Parser::errorPage(405, "Method Not Allowed");;
}
std::string Response::InternalServerError()
{
	return Parser::errorPage(500, "Internal Server Error");
}
std::string Response::BadRequest()
{
	return Parser::errorPage(400, "Bad Request");
}
std::string Response::render(const std::string filePath, std::map<std::string, std::string> context, const std::string status, const size_t statusCode)
{
	std::string html = Parser::parseTemplate(Parser::readFile(filePath), context);
	return Parser::makeResponse(html, status, statusCode, "html");
}
std::string Response::responseStatic(const std::string fileName)
{
	size_t pos = fileName.find('.');
	std::string type;
	if (pos != std::string::npos)
	{
		type = std::string(fileName.begin() + pos + 1, fileName.end());
	}
	else
	{
		throw "bad request";
	}
	return Parser::makeResponse(Parser::readFile(fileName), "OK", 200, type);
}
