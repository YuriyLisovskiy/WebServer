#pragma once

#ifndef WEB_SERVER_VIEW_H
#define WEB_SERVER_VIEW_H

#include "response.h"
#include "request.h"

class View
{
public:
	explicit View(std::string templateDir = "", std::string staticDir = "");
	virtual std::string Get(http::Request& request);
	virtual std::string Post(http::Request& request);
	virtual std::string Put(http::Request& request);
	virtual std::string Delete(http::Request& request);
	virtual std::string Head(http::Request& request);
	virtual std::string Connect(http::Request& request);
	virtual std::string Options(http::Request& request);
	virtual std::string Trace(http::Request& request);
	virtual std::string Patch(http::Request& request);
	void setTemplateDir(std::string dir);
	void setStaticDir(std::string dir);
	bool urlIsAvailable(std::string url);
	bool hasStatic(std::string fileName);
	std::string createStaticDir(const std::string url);
protected:
	std::string url;
	std::string templateDir;
	std::string staticDir;
};

#endif
