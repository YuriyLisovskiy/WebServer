#pragma once

#ifndef WEBSERVER_BASEVIEW_H
#define WEBSERVER_BASEVIEW_H

#include "HttpRequest.h"
#include <string>
#include <vector>

class BaseView
{
protected:
	std::string url;
	std::string dir;

public:
	explicit BaseView(const std::string directory = "");
	virtual std::string Get(Request& request);
	virtual std::string Post(Request& request);
	virtual std::string Put(Request& request);
	virtual std::string Delete(Request& request);
	bool hasUrl(const std::string url);
	void setCustomDir(const std::string dir);
};


#endif //WEBSERVER_BASEVIEW_H
