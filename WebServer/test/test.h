#pragma once
#include "../src/include/BaseView.h"
#include "../src/include/HttpResponse.h"
#include "../src/include/HttpServer.h"

class TestView : public BaseView
{
public:
	TestView() : BaseView("test/")
	{
		this->url = "site/welcome";
	};
	std::string Get(Request& request) final
	{
		return Response::HttpResponse(this->dir + "test.html");
	}
};

void TEST()
{
	HttpServer server;
	server.setView(new TestView());
	server.run();
}
