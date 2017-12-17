#pragma once
#include <string>

class Response
{
public:
	static std::string HttpResponse(const std::string filePath, const std::string status = "OK", const size_t statusCode = 200);
	static std::string NotFound();
	static std::string Forbidden();
	static std::string MethodNotAllowed();
	static std::string InternalServerError();
private:
	static std::string errorPage(const size_t code, const std::string msg);
};
