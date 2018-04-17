#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <set>
#include <unordered_map>
#include "definitions.h"

std::vector<std::string> split(std::string str, char c)
{
  std::vector<std::string> ret;
  std::stringstream ss{str};
  std::string buf;
  while (std::getline(ss, buf, c)) {
    ret.push_back(buf);
  }
  
  return ret;
}

std::unordered_map<std::string, std::string> fromJson(std::string str)
{
	std::unordered_map<std::string, std::string> hash;

	if (str[0] == '{')
	{
		str = str.substr(1);
	}
	if (str[str.length() - 1] == '}')
	{
		str.pop_back();
	}

	size_t startIndex = 0;
	size_t index;
	while (true)
	{
		index = str.find(':', startIndex);
		if (index == std::string::npos)
		{
			break;
		}
		std::string key = str.substr(startIndex, index - startIndex);
		
		startIndex = index + 1;
		if (str[startIndex] == '{')
		{
			index = str.find('}', startIndex);
			index = str.find(',', index);
		}
		else
		{
			index = str.find(',', startIndex);
		}

		if (index == std::string::npos)
		{
			hash[key] = str.substr(startIndex);
			break;
		}

		hash[key] = str.substr(startIndex, index - startIndex);

		startIndex = index + 1;
	}

	/*
	std::list<std::string> splited;
	std::stringstream ss{ str };
	std::string buf;
	while (std::getline(ss, buf, ',')) {
		splited.push_back(buf);
	}

	for (std::list<std::string>::const_iterator ite = splited.begin(); ite != splited.end(); ++ite)
	{
		size_t index = ite->find(":");
		std::string key = ite->substr(0, index);
		std::string value = ite->substr(index + 1);
		hash[key] = value;
	}
	*/
	return hash;
}