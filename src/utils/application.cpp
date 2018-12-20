#include "../include/application.h"
#include <regex>
#include <fstream>

Application::Application(const std::string& root, const std::string& templateDir, const std::string& staticDir)
{
	this->rootDir = root;
	this->templateDir = BASE_DIR + "/" + this->rootDir + templateDir;
	this->staticDir = BASE_DIR + "/" + this->rootDir + staticDir;
}

bool Application::hasStatic(const std::string& fileName)
{
	std::ifstream file(this->staticDir + fileName);
	bool res = false;
	if (file.is_open())
	{
		res = true;
		file.close();
	}
	return res;
}

std::string Application::createStaticDir(const std::string& url)
{
	return this->staticDir + url;
}

bool Application::checkUrl(const std::string& url)
{
	return this->urls.urlExists(url);
}

appFunc Application::getFunction(const std::string& key)
{
	return this->urls.urls[key];
}
