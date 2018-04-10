#include <iostream>
#include <sstream>
#include <vector>
#include <list>
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
	
	return hash;
}