#pragma once

#ifndef WEB_SERVER_LOGGER_H
#define WEB_SERVER_LOGGER_H

#include <mutex>
#include <string>

class Logger
{
private:
	static Logger* instance;
	Logger() = default;
	Logger(const Logger& copy) = default;
public:
	static std::string path;

	static Logger* log();
	void file(const std::string& msg, std::mutex& lock);
	void info(const std::string& msg, std::mutex& lock);
	void error(const std::string& msg, int line, std::mutex& lock);
};

#endif
