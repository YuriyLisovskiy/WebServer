#include "site.h"
#include <iostream>

using namespace HTTP;

int main(int argc, char* argv[])
{
	try
	{
		char* port;
		if (argc > 1)
		{
			port = argv[1];
		}
		else
		{
			port = START_PORT;
		}
		HttpServer server(&db, port);
		server.setView(new TestView());
		server.run();
	}
	catch (const std::exception& exc)
	{
		std::cout << exc.what();
	}
	catch (...)
	{
		std::cout << "Error...";
	}
	std::cin.get();
	return 0;
}
