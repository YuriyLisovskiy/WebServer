#include "../include/ServerMacros.h"

std::string getRootDir()
{
	std::string dir(getcwd(nullptr, 0));
	return std::string(dir.begin(), dir.begin() + dir.rfind('/') + 1) + "WebServer/";
}
