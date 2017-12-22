#pragma once

#ifndef WEB_SERVER_HTTP_RESPONSE_H
#define WEB_SERVER_HTTP_RESPONSE_H

#include <string>

class Response
{
public:
	static std::string HttpResponse(const std::string filePath, const std::string status = "OK", const size_t statusCode = 200);
	static std::string NotFound();
	static std::string Forbidden();
	static std::string MethodNotAllowed();
	static std::string InternalServerError();
	static std::string BadRequest();
private:
	static std::string errorPage(const size_t code, const std::string msg);
};

#endif
