#pragma once
#include "../src/include/View.h"
#include "../src/include/HttpResponse.h"
#include "../src/include/HttpServer.h"

class TestView : public View
{
public:
	TestView() : View("test/")
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
	system("pause");
}
