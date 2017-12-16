#include "../include/View.h"
#include "../include/HtmlResponse.h"
#include "../include/Header.h"

View::View(const std::string directory)
{
	this->dir = BASE_DIR + directory;
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
bool View::hasUrl(const std::string url)
{
	return this->urls.find(url) != this->urls.end();
}
