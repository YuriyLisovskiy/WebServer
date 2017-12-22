#include "site.h"

void main()
{
	HttpServer server(&db);
	server.setView(new TestView());
	server.run();
}
