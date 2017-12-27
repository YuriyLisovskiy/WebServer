#pragma once
#include "../src/include/view.h"
#include "../src/include/response.h"

using http::Request;
using http::Response;
using std::string;

class TestView : public View
{
public:
	TestView() : View("test/", "test/static/")
	{
		this->url = "welcome/";
	};
	string Get(Request& request) final
	{
		return Response::HttpResponse(this->templateDir + "test.html");
	}
};
