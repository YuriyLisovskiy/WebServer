#include "../include/application.h"
#include <regex>
#include <fstream>

Application::Application(const std::string& templateDir, const std::string& staticDir)
{
	this->setTemplateDir(BASE_DIR + templateDir);
	this->setStaticDir(BASE_DIR + staticDir);
}
void Application::setTemplateDir(const std::string& dir)
{
	this->templateDir = dir;
}
void Application::setStaticDir(const std::string& dir)
{
	this->staticDir = dir;
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
