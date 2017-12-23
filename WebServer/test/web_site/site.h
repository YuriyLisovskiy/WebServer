#pragma once
#include "../../src/include/BaseView.h"
#include "../../src/include/HttpResponse.h"
#include "../../src/include/HttpServer.h"

static SimpleDB db(BASE_DIR + "test/web_site/test.db");

class TestView : public BaseView
{
public:
	TestView() : BaseView("test/web_site/")
	{
		this->url = "";
	};
	std::string Get(Request& request) final
	{
		std::vector<std::string> dates = db.read("statistic_date");
		std::vector<std::string> speeds = db.read("statistic_speed");
		std::vector<std::string> likes = db.read("stars");
		std::string starsNumber = "0";
		if (!likes.empty())
		{
			starsNumber = likes.front();
		}
		std::string buttonText("Star");
		if (db.exists({ "client_liked", request.DATA.get("client") }))
		{
			buttonText = "Unstar";
		}
		std::string chartLabels(this->makeArray(dates));
		std::string chartValues(this->makeArray(speeds, false));
		std::map<std::string, std::string> context = {
			{ "stars", starsNumber },
			{ "button_text", buttonText },
			{ "chart_values", chartValues },
			{ "chart_labels", chartLabels },
			{ "requests_total", db.readUnique("requests_total") }
		};
		return Response::render(this->templateDir + "template.html", context);
	}
	std::string Post(Request& request) final
	{
		size_t starsNumber = 0;
		std::vector<std::string> stars = db.read("stars");
		std::string buttonText("Star");
		if (!stars.empty())
		{
			starsNumber = std::stoi(stars.front());
		}
		std::string client = request.DATA.get("client");
		if (!db.exists({ "client_liked", client }))
		{
			buttonText = "Unstar";
			db.write({ "client_liked", client }, true);
			std::pair<std::string, std::string> newPair("stars", std::to_string(starsNumber));
			if (db.exists(newPair))
			{
				db.replace(newPair, std::to_string(starsNumber + 1));
			}
			else
			{
				db.write({ "stars", std::to_string(starsNumber + 1) }, true);
			}
			starsNumber++;
		}
		else
		{
			db.remove({ "client_liked", client });
			std::pair<std::string, std::string> newPair("stars", std::to_string(starsNumber));
			if (db.exists(newPair))
			{
				db.replace({ "stars", std::to_string(starsNumber) }, std::to_string(starsNumber - 1));
			}
			else
			{
				db.write({ "stars", std::to_string(starsNumber - 1) }, true);
			}
			starsNumber--;
		}
		std::vector<std::string> dates = db.read("statistic_date");
		std::vector<std::string> speeds = db.read("statistic_speed");
		std::string statistic(""), lastRequest("");
		std::string chartLabels(this->makeArray(dates));
		std::string chartValues(this->makeArray(speeds, false));
		std::map<std::string, std::string> context = {
			{ "stars", std::to_string(starsNumber) },
			{ "button_text", buttonText },
			{ "chart_values", chartValues },
			{ "chart_labels", chartLabels },
			{ "requests_total", db.readUnique("requests_total") }
		};
		return Response::render(this->templateDir + "template.html", context, "Edited", 201);
	}
	std::string makeArray(std::vector<std::string> arr, bool isStrings = true)
	{
		std::string symbol = "\"";
		if (!isStrings)
		{
			symbol = "";
		}
		std::string result("[");
		if (!arr.empty())
		{
			auto itBegin = arr.begin(), itEnd = arr.end() - 1;
			while (itBegin != itEnd)
			{
				result += symbol + *itBegin + symbol + ", ";
				itBegin++;
			}
			result += symbol + arr.back() + symbol;
		}
		result += "]";
		return result;
	}
};
