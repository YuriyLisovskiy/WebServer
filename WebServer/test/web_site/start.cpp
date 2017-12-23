#include "site.h"

using namespace HTTP;

int main()
{
	HttpServer server(&db);
	server.setView(new TestView());
	server.run();
	return 0;
}
