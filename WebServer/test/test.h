#pragma once
#include "../src/include/View.h"
#include "../src/include/HtmlResponse.h"
#include "../src/include/HttpServer.h"

class TestView : public View
{
public:
	TestView() : View("test/")
	{
		this->urls.push_back("site/welcome");
	};
	std::string Get(Request& request) final
	{
		return HTMLResponse::HttpResponse(this->dir + "test.html");
	}
};

void TEST()
{
	HttpServer server;
	server.setView(new TestView());
	server.startServer();
}
