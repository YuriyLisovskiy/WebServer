#pragma once

#ifndef WEB_SERVER_HELPERS_H
#define WEB_SERVER_HELPERS_H

#include <string>
#include <vector>

class SimpleDB
{
private:
	std::string db;
public:
	explicit SimpleDB(const std::string& db);
	void write(std::pair<std::string, std::string> data, bool unique = false);
	std::vector<std::string> read(std::string keyword);
};


#endif
