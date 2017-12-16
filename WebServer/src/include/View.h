#pragma once
#include "HttpRequest.h"
#include <string>

class View
{
private:
	std::map<std::string, std::string> urls;
	std::string dir;

public:
	View(const std::string directory = "");
	void setUrls(const std::map<std::string, std::string>& urls);
	virtual std::string Get(Request& request);
	virtual std::string Post(Request& request);
	virtual std::string Put(Request& request);
	virtual std::string Delete(Request& request);
};
