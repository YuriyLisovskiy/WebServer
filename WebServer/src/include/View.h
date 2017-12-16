#pragma once
#include "HttpRequest.h"
#include <string>

class View
{
protected:
	std::map<std::string, std::string> urls;
	std::string dir;

public:
	View(const std::string directory = "");
	virtual std::string Get(Request& request);
	virtual std::string Post(Request& request);
	virtual std::string Put(Request& request);
	virtual std::string Delete(Request& request);
	bool hasUrl(const std::string url);
};
