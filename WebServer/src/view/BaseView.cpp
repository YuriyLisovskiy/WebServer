#include "../include/BaseView.h"
#include "../include/HttpResponse.h"
#include "../include/ServerMacros.h"

BaseView::BaseView(const std::string dir)
{
	this->url = "";
	this->setCustomDir(BASE_DIR + dir);
}
void BaseView::setCustomDir(const std::string dir)
{
	this->dir = dir;
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
bool BaseView::hasUrl(const std::string url)
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
	return urlToCompare == url;
}
