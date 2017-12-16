#pragma once
#include "HttpRequest.h"
#include <string>
#include <vector>

class View
{
protected:
	std::vector<std::string> urls;
	std::string dir;

public:
	View(const std::string directory = "");
	virtual std::string Get(Request& request);
	virtual std::string Post(Request& request);
	virtual std::string Put(Request& request);
	virtual std::string Delete(Request& request);
	bool hasUrl(const std::string url);
	void setDirectory(const std::string dir);
};
