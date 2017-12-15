#pragma once
#include <string>

class HTMLResponse
{
public:
	static std::string generate(const size_t statusCode, const std::string status, const std::string html);
	static std::string notFound();
	static std::string forbidden();
	static std::string methodNotAllowed();
	static std::string internalServerError();
private:
	static std::string errorPage(const size_t code, const std::string msg);
};
