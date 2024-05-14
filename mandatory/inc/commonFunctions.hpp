#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "toString.hpp"

std::string removeBlanksAndTabs(const std::string& input);
std::string removeCharFromString(const std::string& input, char c);
std::vector<std::string> splitString(const std::string &str, char delimiter);
size_t stringToSizeT(const std::string& str);
std::map<std::string, std::string> create_filetypes();
void replaceString(std::string &mainString, const std::string &searchString, const std::string &replaceString);
