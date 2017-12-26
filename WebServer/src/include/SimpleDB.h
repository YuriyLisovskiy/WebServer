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
	std::string readUnique(std::string keyword);
	void remove(std::pair<std::string, std::string> data);
	bool exists(std::pair<std::string, std::string> data);
	void replace(std::pair<std::string, std::string> oldData, std::string newValue);
	void setDB(const std::string& db);
};

#endif