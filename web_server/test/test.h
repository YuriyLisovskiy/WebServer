#pragma once
#include "../src/include/application.h"
#include "../src/include/response.h"

using http::Request;
using http::Response;
using std::string;

class TestApp : public Application
{
public:
	TestApp() : Application("test/", "test/static/")
	{
		std::vector<std::pair<std::string, func>> urlPatterns = {
			{ "welcome/", std::bind(&TestApp::index, this, std::placeholders::_1) }
		};
		this->urls.set(urlPatterns);
	};
	std::string index(Request& request)
	{
		return Response::HttpResponse(this->templateDir + "test.html");
	}
};
