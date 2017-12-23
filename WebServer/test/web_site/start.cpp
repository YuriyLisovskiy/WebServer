#include "site.h"

using namespace HTTP;

void main()
{
	HttpServer server(&db);
	server.setView(new TestView());
	server.run();
}
