#pragma once

#ifndef WEB_SERVER_BASE_VIEW_H
#define WEB_SERVER_BASE_VIEW_H

#include "HttpRequest.h"
#include <string>
#include <vector>

class BaseView
{
protected:
	std::string url;
	std::string templateDir;

public:
	explicit BaseView(const std::string directory = "", const std::string staticDir = "");
	virtual std::string Get(Request& request);
	virtual std::string Post(Request& request);
	virtual std::string Put(Request& request);
	virtual std::string Delete(Request& request);
	bool hasUrl(const std::string url);
	void setTemplateDir(const std::string dir);
};

#endif
