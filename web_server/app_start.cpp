#include "src/include/server.h"
#include "demo/demo.h"
#include <iostream>

using http::Server;
using std::cin;

int main(int argc, char* argv[])
{
    Server server(argc, argv);
    server.setApp(new DemoApp());
    server.start();
	cin.get();
	return 0;
}