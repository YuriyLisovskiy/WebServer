#include <iostream>
#include <fstream>

#include "../include/logger.h"

Logger* Logger::log()
{
	if (Logger::instance == nullptr)
	{
		Logger::instance = new Logger();
	}
	return Logger::instance;
}

void Logger::file(const std::string& msg, std::mutex& lock)
{
	lock.lock();
	std::ofstream file(Logger::path, std::ios::app);
	if (file.is_open())
	{
		file << msg;
		file.close();
	}
	lock.unlock();
}

void Logger::info(const std::string& msg, std::mutex& lock)
{
	lock.lock();
	std::cout << "\n[INFO]:\n Message: \"" << msg << "\"\n\n";
	lock.unlock();
}

void Logger::error(const std::string& msg, const int line, std::mutex& lock)
{
	lock.lock();
	std::cout << "\n[ERROR]:\n Message: \"" << msg << "\", line: " << line << "\n\n";
	lock.unlock();
}

Logger* Logger::instance = nullptr;
