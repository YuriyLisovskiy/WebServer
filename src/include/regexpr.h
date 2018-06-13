#pragma once

#ifndef WEB_SERVER_REG_EXPR_H
#define WEB_SERVER_REG_EXPR_H

#include <string>

namespace regex
{
	static const std::string FIRST_LINE_REQUEST(R"((GET|POST|PUT|DELETE)\s/(.*)\s(HTTP.+))");
	static const std::string REQUEST_GET_PARAM(R"((\w+)=(.+))");
}

#endif

