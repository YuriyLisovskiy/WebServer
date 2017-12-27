#include "../include/view.h"
#include <regex>
#include <fstream>

View::View(const std::string templateDir, const std::string staticDir)
{
	this->url = "";
	this->setTemplateDir(BASE_DIR + templateDir);
	this->setStaticDir(BASE_DIR + staticDir);
}
void View::setTemplateDir(const std::string dir)
{
	this->templateDir = dir;
}
void View::setStaticDir(const std::string dir)
{
	this->staticDir = dir;
}
std::string View::Get(http::Request& request)
{
	return http::Response::MethodNotAllowed();
}
std::string View::Post(http::Request& request)
{
	return http::Response::MethodNotAllowed();
}
std::string View::Put(http::Request& request)
{
	return http::Response::MethodNotAllowed();
}
std::string View::Delete(http::Request& request)
{
	return http::Response::MethodNotAllowed();
}
std::string View::Connect(http::Request& request)
{
	return http::Response::MethodNotAllowed();
}
std::string View::Head(http::Request& request)
{
	return http::Response::MethodNotAllowed();
}
std::string View::Options(http::Request& request)
{
	return http::Response::MethodNotAllowed();
}
std::string View::Patch(http::Request& request)
{
	return http::Response::MethodNotAllowed();
}
std::string View::Trace(http::Request& request)
{
	return http::Response::MethodNotAllowed();
}
bool View::urlIsAvailable(const std::string url)
{
	std::string urlToCompare(this->url);
	if (!this->url.empty())
	{
		while (urlToCompare.back() == '/')
		{
			urlToCompare.pop_back();
		}
		if (urlToCompare.front() == '/')
		{
			urlToCompare = std::string(urlToCompare.begin() + 1, urlToCompare.end());
		}
	}
	return urlToCompare == url || this->hasStatic(url);
}
bool View::hasStatic(const std::string fileName)
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
std::string View::createStaticDir(const std::string url)
{
	return this->staticDir + url;
}
