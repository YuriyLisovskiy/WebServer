#include "../include/View.h"
#include "../include/HtmlResponse.h"
#include "../include/Header.h"

View::View(const std::string directory)
{
	this->dir = BASE_DIR + directory;
}
void View::setUrls(const std::map<std::string, std::string>& urls)
{
	this->urls = urls;
}
std::string View::Get(Request& request)
{
	return HTMLResponse::MethodNotAllowed();
}
std::string View::Post(Request& request)
{
	return HTMLResponse::MethodNotAllowed();
}
std::string View::Put(Request& request)
{
	return HTMLResponse::MethodNotAllowed();
}
std::string View::Delete(Request& request)
{
	return HTMLResponse::MethodNotAllowed();
}
