#include "../include/application.h"

void Application::Urls::set(std::vector<std::pair<std::string, func>> urls)
{
	if (!urls.empty())
	{
		for (const auto& url : urls)
		{
			std::string newUrl(url.first);
			while (newUrl.back() == '/')
			{
				newUrl.pop_back();
			}
			if (newUrl.front() == '/')
			{
				newUrl = std::string(newUrl.begin() + 1, newUrl.end());
			}
			this->urls[newUrl] = url.second;
		}
	}
}
bool Application::Urls::urlExists(const std::string& url)
{
	return this->urls.find(url) != this->urls.end();
}
