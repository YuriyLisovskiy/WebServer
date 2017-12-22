#pragma once
#include "../src/include/BaseView.h"
#include "../src/include/HttpResponse.h"
#include "../src/include/HttpServer.h"
#include <iostream>

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
		std::vector<std::string> data = db.read("statistic");
		std::string statistic("");
		for (std::vector<std::string>::iterator it = data.begin(); it < data.end() - 1; it++)
		{
			statistic += "<p>" + *it + "</p>\n";
		}
		statistic += "<h3>Last request:</h3>\n";
		statistic += "<p>" + data.back() + "</p>";
		std::map<std::string, std::string> context = {
			{"statistic", statistic }
		};
		return Response::render(this->dir + "test.html", context);
	}
	std::string Post(Request& request) final
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

void TEST_DB()
{
	SimpleDB testDB(BASE_DIR + "test/test.db");
	testDB.write({"table_1", "some_data_1"}, true);
//	testDB.write({"table_2", "some_data_2"}, true);
	testDB.write({"table_1", "some_data_3"}, true);
//	testDB.write({"table_2", "some_data_4"}, true);
//	testDB.write({"table_2", "some_data_4"}, true);

	std::vector<std::string> data = testDB.read("table_1");
	testDB.remove({ "table_1", "some_data_1" });
	std::cout << "First table:\n";
	for (const auto& val : data)
	{
		std::cout << val << "\n";
	}
	std::cout << "\n\n";
	data.clear();
	data = testDB.read("table_1");
	std::cout << "First table:\n";
	for (const auto& val : data)
	{
		std::cout << val << "\n";
	}
}
