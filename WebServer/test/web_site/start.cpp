#include "site.h"
#include <iostream>

using namespace HTTP;

int main()
{
	HttpServer server(&db);
	server.setView(new TestView());
	try
	{
		server.run();
	}
	catch (const std::exception& exc)
	{
		std::cout << exc.what();
		std::cin.get();
	}
	catch (...)
	{
		std::cout << "Error...";
		std::cin.get();
	}
	return 0;
}
