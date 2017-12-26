#include "../include/ServerMacros.h"

#if defined(__unix) || defined(__unix__) || defined(__APPLE__) || defined(__MACH__) || defined(__linux__) || defined(__FreeBSD__)

std::string getRootDir()
{
	std::string dir(getcwd(nullptr, 0));
	return std::string(dir.begin(), dir.begin() + dir.rfind('/') + 1) + "WebServer/";
}

#endif
