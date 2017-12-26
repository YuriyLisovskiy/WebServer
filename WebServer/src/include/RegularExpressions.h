#pragma once

#ifndef WEB_SERVER_REGULAR_EXPRESSIONS_H
#define WEB_SERVER_REGULAR_EXPRESSIONS_H

#include <string>

namespace REGEX
{
	static const std::string FIRST_LINE_REQUEST(R"((GET|POST|PUT|DELETE)\s/(.*)\s(HTTP.+))");
	static const std::string REQUEST_GET_PARAM(R"((\w+)=(.+))");
}

#endif
