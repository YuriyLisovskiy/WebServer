#pragma once

#ifndef WEB_SERVER_APPLICATION_H
#define WEB_SERVER_APPLICATION_H

#include "response.h"
#include "request.h"
#include <map>
#include <vector>
#include <functional>

using func = std::function<std::string(http::Request&)>;

class Application
{
private:
	class Urls
	{
		friend class Application;
	private:
		std::map<std::string, func> urls;
	public:
		void set(std::vector<std::pair<std::string, func>> urls);
		bool urlExists(const std::string& url);
	};
public:
	explicit Application(const std::string& templateDir = "", const std::string& staticDir = "");
	void setTemplateDir(const std::string& dir);
	void setStaticDir(const std::string& dir);
	bool hasStatic(const std::string& fileName);
	std::string createStaticDir(const std::string& url);
	bool checkUrl(const std::string& url);
	func getFunction(const std::string& key);
protected:
	Urls urls;
	std::string templateDir;
	std::string staticDir;
};

#endif
