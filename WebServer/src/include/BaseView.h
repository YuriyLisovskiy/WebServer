#pragma once

#ifndef WEB_SERVER_BASE_VIEW_H
#define WEB_SERVER_BASE_VIEW_H

#include "HttpRequest.h"
#include <string>
#include <vector>

class BaseView
{
public:
	explicit BaseView(const std::string templateDir = "", const std::string staticDir = "");
	virtual std::string Get(Request& request);
	virtual std::string Post(Request& request);
	virtual std::string Put(Request& request);
	virtual std::string Delete(Request& request);
	void setTemplateDir(const std::string dir);
	void setStaticDir(const std::string dir);
	bool urlIsAvailable(const std::string url);
	bool hasStatic(const std::string fileName);
	std::string createStaticDir(const std::string url);
protected:
	std::string url;
	std::string templateDir;
	std::string staticDir;
};

#endif
