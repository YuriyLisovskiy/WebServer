cmake_minimum_required(VERSION 3.9)
project(WebServer)

set(CMAKE_CXX_STANDARD 11)

SET( CMAKE_CXX_FLAGS  "-pthread")

add_executable(WebServer
		WebServer/src/include/CommonMacros.h
		WebServer/src/include/HttpRequest.h
		WebServer/src/include/HttpResponse.h
		WebServer/src/include/HttpServer.h
		WebServer/src/include/RegularExpressions.h
		WebServer/src/include/RequestMacros.h
		WebServer/src/include/ServerMacros.h
		WebServer/src/parser/RequestParser.cpp
		WebServer/src/parser/ServerParser.cpp
		WebServer/src/server/HttpServer.cpp
		WebServer/src/utils/HttpRequest.cpp
		WebServer/src/utils/HttpResponse.cpp
		WebServer/test/test.cpp
		WebServer/test/test.h
		WebServer/src/utils/Helper.cpp
		WebServer/src/view/BaseView.cpp
		WebServer/src/include/BaseView.h
		WebServer/src/include/SimpleDB.h
		WebServer/src/utils/SimpleDB.cpp
		WebServer/src/parser/ResponseParser.cpp
		WebServer/test/web_site/site.h
		WebServer/test/web_site/start.cpp)
