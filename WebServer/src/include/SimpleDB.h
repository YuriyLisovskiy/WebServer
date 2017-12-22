#pragma once

#ifndef WEB_SERVER_SIMPLE_DB_H
#define WEB_SERVER_SIMPLE_DB_H

#include <string>
#include <vector>

class SimpleDB
{
private:
	std::string db;
public:
	SimpleDB() = default;
	explicit SimpleDB(const std::string& db);
	void write(std::pair<std::string, std::string> data, bool unique = false);
	std::vector<std::string> read(std::string keyword);
	void remove(std::pair<std::string, std::string> data);
	void setDB(const std::string& db);
};

#endif
