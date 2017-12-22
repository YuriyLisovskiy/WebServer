#include "../include/SimpleDB.h"
#include <fstream>
#include <iostream>
#include <regex>

SimpleDB::SimpleDB(const std::string& db)
{
	this->db = db;
	std::ifstream fileRead(this->db);
	if (!fileRead.is_open())
	{
		std::ofstream fileCreate(this->db);
		fileCreate.close();
	}
	else
	{
		fileRead.close();
	}
}
void SimpleDB::setDB(const std::string& db)
{
	this->db = db;
}
void SimpleDB::write(std::pair<std::string, std::string> data, bool unique)
{
	std::ifstream fileRead(this->db);
	std::string dbData("");
	if (fileRead.is_open())
	{
		dbData.assign((std::istreambuf_iterator<char>(fileRead)), std::istreambuf_iterator<char>());
		fileRead.close();
		size_t pos = dbData.find(data.first + "{\n");
		if (pos != std::string::npos)
		{
			pos += data.first.size() + 2;
			size_t posToInsert = dbData.find('}', pos);
			if (!unique)
			{
				dbData.insert(posToInsert, data.second + "\r\n");
			}
			else
			{
				std::string search(dbData.begin() + pos, dbData.begin() + posToInsert);
				posToInsert = search.find(data.second);
				if (posToInsert != std::string::npos)
				{
					posToInsert = data.second.size() + 2;
				}
				else
				{
					posToInsert = 0;
				}
				dbData.insert(pos + posToInsert, data.second + "\r\n");
			}
		}
		else
		{
			dbData.append(data.first + "{\n" + data.second + "\r\n}\n");
		}
	}
	else
	{
		std::cerr << "Error occurred in 'SimpleDB::write()' while reading db: cannot open '" << db << "' for reading.\n";
	}
	if (!dbData.empty())
	{
		std::ofstream fileWrite(this->db);
		if (fileWrite.is_open())
		{
			fileWrite << dbData;
			fileWrite.close();
		}
		else
		{
			std::cerr << "Error occurred in 'SimpleDB::write()' while writing to db: cannot open '" << db << "' for writing.\n";
		}
	}
}
std::vector<std::string> SimpleDB::read(std::string keyword)
{
	std::vector<std::string> list;
	std::ifstream file(this->db);
	if (file.is_open())
	{
		std::string dbData;
		dbData.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();
		size_t pos = dbData.find(keyword + "{");
		if (pos != std::string::npos)
		{
			size_t posEnd = dbData.find('}', pos + keyword.size());
			std::string dataToParse(dbData.begin() + pos + keyword.size() + 1, dbData.begin() + posEnd);
			pos = 0;
			posEnd = dataToParse.find("\r\n");
			while(posEnd != std::string::npos)
			{
				list.emplace_back(std::string(dataToParse.begin() + pos, dataToParse.begin() + posEnd));
				pos = posEnd + 1;
				posEnd = dataToParse.find("\r\n", pos);
			}
		}
		else
		{
		//	std::cerr << "Error occurred in 'SimpleDB::read()' while searching for db data: keyword '" << keyword << "' does not exist.\n";
		}
	}
	else
	{
		std::cerr << "Error occurred in 'SimpleDB::read()' while reading db: cannot open '" << db << "' for reading.\n";
	}
	return list;
}
void SimpleDB::remove(std::pair<std::string, std::string> data)
{
	std::ifstream fileRead(this->db);
	std::string dbData("");
	if (fileRead.is_open())
	{
		dbData.assign((std::istreambuf_iterator<char>(fileRead)), std::istreambuf_iterator<char>());
		fileRead.close();
		size_t pos = dbData.find(data.first + "{");
		if (pos != std::string::npos)
		{
			pos += data.first.size() + 2;
			size_t posToDel = dbData.find('}', pos);
			std::string search(dbData.begin() + pos, dbData.begin() + posToDel);
			posToDel = search.find(data.second);
			if (posToDel != std::string::npos)
			{
				posToDel += pos;
				dbData.erase(dbData.begin() + posToDel, dbData.begin() + posToDel + data.second.size() + 2);
			}
		}
	}
	else
	{
		std::cerr << "Error occurred in 'SimpleDB::write()' while reading db: cannot open '" << db << "' for reading.\n";
	}
	if (!dbData.empty())
	{
		std::ofstream fileWrite(this->db);
		if (fileWrite.is_open())
		{
			fileWrite << dbData;
			fileWrite.close();
		}
		else
		{
			std::cerr << "Error occurred in 'SimpleDB::write()' while writing to db: cannot open '" << db << "' for writing.\n";
		}
	}
}
bool SimpleDB::exists(std::pair<std::string, std::string> data)
{
	std::ifstream fileRead(this->db);
	std::string dbData("");
	if (fileRead.is_open())
	{
		dbData.assign((std::istreambuf_iterator<char>(fileRead)), std::istreambuf_iterator<char>());
		fileRead.close();
		size_t pos = dbData.find(data.first + "{");
		if (pos != std::string::npos)
		{
			pos += data.first.size() + 2;
			size_t posToFind = dbData.find('}', pos);
			std::string search(dbData.begin() + pos, dbData.begin() + posToFind);
			return std::regex_search(search, std::smatch(), std::regex(data.second));
		}
	}
	return false;
}
void SimpleDB::replace(std::pair<std::string, std::string> oldData, std::string newValue)
{
	if (this->exists(oldData))
	{
		this->remove(oldData);
		this->write({ oldData.first, newValue });
	}
}
