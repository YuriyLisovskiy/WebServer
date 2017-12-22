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
		std::vector<std::string> dates = db.read("statistic_date");
		std::vector<std::string> speeds = db.read("statistic_speed");
		std::string statistic("");
		if (!dates.empty() && !speeds.empty())
		{
			std::vector<std::string>::iterator itDates = dates.begin(), endDates = dates.end() - 1;
			std::vector<std::string>::iterator itSpeeds = speeds.begin(), endSpeeds = speeds.end() - 1;
			while (itDates != endDates && itSpeeds != endSpeeds)
			{
				statistic += "<tr><td>\n" + *itDates + "\n</td>\n";
				statistic += "<td>\n" + *itSpeeds + "\n</td>\n</tr>\n";
				itDates++;
				itSpeeds++;
			}
		}
		std::map<std::string, std::string> context = {
			{"statistic", statistic },
			{"last_request", "<tr><td>\n" + dates.back() + "\n</td>\n<td>\n" + speeds.back() + "\n</td>\n</tr>\n" }
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
	HttpServer server(&db);
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
