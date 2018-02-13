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
        if (argv[1] == "runserver")
        {
            try
            {
                std::string port;
                if (argc > 2)
                {
                    port = argv[2];
                }
                else
                {
                    port = SERVER_PORT;
                }
                Server server(port);
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
        else if (argv[1] == "test")
        {
            // TODO: run tests
        }
    }
    else
    {
        cerr << "Invalid arguments passed...";
    }
	cin.get();
	return 0;
}