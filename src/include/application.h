#pragma once

#ifndef WEB_SERVER_APPLICATION_H
#define WEB_SERVER_APPLICATION_H

#include <vector>
#include <functional>

#include "request.h"
#include "response.h"

using appFunc = std::function<std::string(http::Request&)>;

class Application
{
private:
	class Urls
	{
		friend class Application;
	private:
		std::map<std::string, appFunc> urls;
	public:
		void set(std::vector<std::pair<std::string, appFunc>> urls);
		bool urlExists(const std::string& url);
	};
public:
	explicit Application(const std::string& root = "./", const std::string& templateDir = "./", const std::string& staticDir = "./");
	bool hasStatic(const std::string& fileName);
	std::string createStaticDir(const std::string& url);
	bool checkUrl(const std::string& url);
	appFunc getFunction(const std::string& key);
protected:
	Urls urls;
	std::string templateDir;
	std::string staticDir;
	std::string rootDir;
};

#endif
