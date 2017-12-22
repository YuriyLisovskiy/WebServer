#pragma once
#include "../src/include/BaseView.h"
#include "../src/include/HttpResponse.h"
#include "../src/include/HttpServer.h"

static SimpleDB db(BASE_DIR + "test/test.db");

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
	HttpServer server(db);
	server.setView(new TestView());
	server.run();
}

#include "../src/include/SimpleDB.h"
#include <iostream>

void TEST_DB()
{
	SimpleDB db(BASE_DIR + "test/test.db");
	db.write({"table_1", "some_data_1"}, true);
	db.write({"table_2", "some_data_2"}, true);
	db.write({"table_1", "some_data_3"}, true);
	db.write({"table_2", "some_data_4"}, true);
	db.write({"table_2", "some_data_4"}, true);

	std::vector<std::string> data = db.read("table_1");
	std::cout << "First table:\n";
	for (const auto& val : data)
	{
		std::cout << val << "\n";
	}
	std::cout << "\n\n";
	data.clear();
	data = db.read("table_2");
	std::cout << "Second table:\n";
	for (const auto& val : data)
	{
		std::cout << val << "\n";
	}
	std::cout << "\n\n";
}
