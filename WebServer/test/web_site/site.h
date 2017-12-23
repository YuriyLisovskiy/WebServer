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
		std::string statistic(""), lastRequest("");
		if (!dates.empty() && !speeds.empty())
		{
			std::vector<std::string>::iterator itDates = dates.begin(), endDates = dates.end() - 1;
			std::vector<std::string>::iterator itSpeeds = speeds.begin(), endSpeeds = speeds.end() - 1;
			while (itDates != endDates && itSpeeds != endSpeeds)
			{
				statistic += "<tr><td>" + *itDates + "</td>\n";
				statistic += "<td>" + *itSpeeds + "</td>\n</tr>\n";
				itDates++;
				itSpeeds++;
			}
			lastRequest = "<tr>\n<td>" + dates.back() + "</td>\n<td>" + speeds.back() + "</td>\n</tr>\n";
		}
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
		std::map<std::string, std::string> context = {
			{ "statistic", statistic },
			{ "last_request", lastRequest },
			{ "stars", starsNumber },
			{ "button_text", buttonText }
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
		if (!dates.empty() && !speeds.empty())
		{
			std::vector<std::string>::iterator itDates = dates.begin(), endDates = dates.end() - 1;
			std::vector<std::string>::iterator itSpeeds = speeds.begin(), endSpeeds = speeds.end() - 1;
			while (itDates != endDates && itSpeeds != endSpeeds)
			{
				statistic += "<tr>\n<td>" + *itDates + "</td>\n";
				statistic += "<td>" + *itSpeeds + "</td>\n</tr>\n";
				itDates++;
				itSpeeds++;
			}
			lastRequest = "<tr>\n<td>" + dates.back() + "</td>\n<td>" + speeds.back() + "</td>\n</tr>\n";
		}
		std::map<std::string, std::string> context = {
			{ "statistic", statistic },
			{ "last_request", lastRequest },
			{ "stars", std::to_string(starsNumber) },
			{ "button_text", buttonText }
		};
		return Response::render(this->templateDir + "template.html", context, "Edited", 201);
	}
};
