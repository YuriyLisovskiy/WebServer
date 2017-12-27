#include "../src/include/server.h"
#include "test.h"
#include <iostream>

using http::Server;
using std::cerr;
using std::cin;

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
			port = (char*)SERVER_PORT;
		}
		Server server(port);
		server.setView(new TestView());
		server.start();
	}
	catch (const std::exception& exc)
	{
		cerr << exc.what();
	}
	catch (...)
	{
		cerr << "Error...";
	}
	cin.get();
	return 0;
}