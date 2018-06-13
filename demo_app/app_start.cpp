#include "../src/include/server.h"
#include "demo.h"
#include <iostream>

using http::Server;
using std::cin;

int main(int argc, char* argv[])
{
    Server server(argc, argv);
    server.setApp(new DemoApp());
    server.start();
	return 0;
}