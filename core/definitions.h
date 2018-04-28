#ifndef DEFINITIONS_H
#define DEFINITIONS_H

typedef unsigned char uchar;

std::vector<std::string> split(std::string str, char c);
std::unordered_map<std::string, std::string> fromJson(std::string str);
std::list<std::string> fromJsonArray(std::string str);

#endif // DEFINITIONS_H