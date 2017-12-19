#pragma once
#include <string>

namespace REGEX
{
	static const std::string FIRST_LINE_REQUEST(R"((GET|POST|PUT|DELETE)\s/(.*)\s(HTTP.+))");
	static const std::string REQUEST_GET_PARAM(R"((\w+)=(.+))");
}
