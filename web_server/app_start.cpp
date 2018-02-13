#include "src/include/server.h"
#include "demo/demo.h"
#include <iostream>

using http::Server;
using std::cerr;
using std::cin;

int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		if (std::string(argv[1]) == "runserver")
		{
			try
			{
				std::string port, ip;
				if (argc > 3)
				{
					ip = argv[2];
					port = argv[3];
				}
				else
				{
					ip = SERVER_IP;
					port = SERVER_PORT;
				}
				Server server(ip, port);
				server.setApp(new DemoApp());
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
		}
		else if (std::string(argv[1]) == "test")
		{
			std::cout << "Test running...";
		}
		else
		{
			cerr << "Invalid arguments passed...";
		}
	}
	else
	{
		cerr << "Invalid arguments passed...";
	}
	cin.get();
	return 0;
}