#pragma once

#ifndef WEB_SERVER_HTTP_RESPONSE_H
#define WEB_SERVER_HTTP_RESPONSE_H

#include "HttpServer.h"
#include <string>
#include <map>

class Response
{
	friend class HTTP::HttpServer;

public:
	static std::string HttpResponse(const std::string filePath, const std::string status = "OK", const size_t statusCode = 200);
	static std::string render(const std::string filePath, std::map<std::string, std::string> context, const std::string status = "OK", const size_t statusCode = 200);
	static std::string NotFound();
	static std::string Forbidden();
	static std::string MethodNotAllowed();
	static std::string InternalServerError();
	static std::string BadRequest();
private:
	class Parser
	{
	public:
		static std::string parseTemplate(const std::string html, std::map<std::string, std::string> context);
		static std::string readFile(const std::string filePath);
		static std::string errorPage(const size_t code, const std::string msg);
		static std::string makeResponse(const std::string html, const std::string statusStr, const size_t statusCode, const std::string requestContent);
		static std::string setContentType(const std::string requestContent);
	};

	static std::string responseStatic(const std::string fileName);
};

#endif
