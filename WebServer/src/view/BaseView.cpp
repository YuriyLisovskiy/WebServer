#include "../include/BaseView.h"
#include "../include/HttpResponse.h"
#include <regex>

BaseView::BaseView(const std::string templateDir, const std::string staticDir)
{
	this->url = "";
	this->setTemplateDir(BASE_DIR + templateDir);
	this->setStaticDir(BASE_DIR + staticDir);
}
void BaseView::setTemplateDir(const std::string dir)
{
	this->templateDir = dir;
}
void BaseView::setStaticDir(const std::string dir)
{
	this->staticDir = dir;
}
std::string BaseView::Get(Request& request)
{
	return Response::MethodNotAllowed();
}
std::string BaseView::Post(Request& request)
{
	return Response::MethodNotAllowed();
}
std::string BaseView::Put(Request& request)
{
	return Response::MethodNotAllowed();
}
std::string BaseView::Delete(Request& request)
{
	return Response::MethodNotAllowed();
}
bool BaseView::urlIsAvailable(const std::string url)
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
bool BaseView::hasStatic(const std::string fileName)
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
std::string BaseView::createStaticDir(const std::string url)
{
	return this->staticDir + url;
}
