#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <netinet/in.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_SENT_BYTES 8192


template <typename T>
bool isrange(T value, T min, T max);
bool isNumber(std::string str);
bool validIPAddress(std::string ip);
bool isDirPermissions(std::string path, int mode);

bool isFilePermissions(std::string path, int mode);

#include "toString.hpp"

std::string removeBlanksAndTabs(const std::string& input);
std::string removeCharFromString(const std::string& input, char c);
std::vector<std::string> splitString(const std::string &str, char delimiter);
size_t stringToSizeT(const std::string& str);
std::map<std::string, std::string> create_filetypes();
std::map<int, std::string> createHttpErrorCodes();
void pauseRequest(std::string message);
template <typename T>
std::string itos (T n);
void replaceString(std::string &mainString, const std::string &searchString, const std::string &replaceString);
int count_chars(const std::string &str, char c);
