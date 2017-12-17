#include "../include/View.h"
#include "../include/HttpResponse.h"
#include "../include/Header.h"

View::View(const std::string dir)
{
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
	return this->url == url;
}
