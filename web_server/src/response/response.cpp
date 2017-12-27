#include "../include/response.h"

std::string http::Response::HttpResponse(const std::string& filePath, const std::string& status, const size_t statusCode)
{
	return Parser::makeResponse(Parser::readFile(filePath), status, statusCode, "html");
}
std::string http::Response::NotFound()
{
	return Parser::errorPage(404, "Not Found");
}
std::string http::Response::Forbidden()
{
	return Parser::errorPage(403, "Forbidden");;
}
std::string http::Response::MethodNotAllowed()
{
	return Parser::errorPage(405, "Method Not Allowed");;
}
std::string http::Response::InternalServerError()
{
	return Parser::errorPage(500, "Internal Server Error");
}
std::string http::Response::BadRequest()
{
	return Parser::errorPage(400, "Bad Request");
}
std::string http::Response::responseStatic(const std::string& fileName)
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
