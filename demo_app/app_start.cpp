#include <iostream>

#include "demo.h"
#include "../src/include/server.h"

using http::Server;
using std::cin;

int main(int argc, char* argv[])
{
    Server server(argc, argv);
    server.setApp(new DemoApp());
    server.start();
	return 0;
}
