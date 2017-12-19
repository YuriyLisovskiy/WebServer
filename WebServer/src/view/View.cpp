#include "../include/View.h"
#include "../include/HttpResponse.h"
#include "../include/ServerMacros.h"

View::View(const std::string dir)
{
	this->url = "";
	this->setCustomDir(BASE_DIR + dir);
}
void View::setCustomDir(const std::string dir)
{
	this->dir = dir;
}
std::string View::Get(Request& request)
{
	return Response::MethodNotAllowed();
}
std::string View::Post(Request& request)
{
	return Response::MethodNotAllowed();
}
std::string View::Put(Request& request)
{
	return Response::MethodNotAllowed();
}
std::string View::Delete(Request& request)
{
	return Response::MethodNotAllowed();
}
bool View::hasUrl(const std::string url)
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
