#include "../include/View.h"
#include "../include/HtmlResponse.h"
#include "../include/Header.h"

View::View(const std::string dir)
{
	this->setDirectory(BASE_DIR + dir);
}
void View::setDirectory(const std::string dir)
{
	this->dir = dir;
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
	for (const auto& currentUrl : this->urls)
	{
		if (currentUrl == url)
		{
			return true;
		}
	}
	return false;
}
