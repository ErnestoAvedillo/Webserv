/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 13:49:35 by eavedill          #+#    #+#             */
/*   Updated: 2024/06/30 15:03:42 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <netinet/in.h>
#include <sys/stat.h>
#include <unistd.h>
#include "colors.h"
#include <iostream>
#include <netinet/in.h>
#include <cstdlib>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream>
#include <ctime>
#include <algorithm>
#define MAX_SENT_BYTES 16048


template <typename T>
bool isrange(T value, T min, T max);

template <typename T>
std::string toString (T n)
{
	std::string str;
	std::stringstream ss;
	ss << n;
	ss >> str;
	return  str;
}

template <typename T>
T minimum(T a, T b)
{
	if(a < b)
		return a;
	return b;
};

template <typename T>
T maximum(T a, T b)
{
	if (a > b)
		return a;
	return b;
};

bool								isNumber(std::string str);
bool								validIPAddress(std::string ip);
bool								isDirPermissions(std::string path, int mode);
int									isFilePermissions(std::string path, int mode);
std::string							removeBlanksAndTabs(const std::string& input);
std::string							removeCharFromString(const std::string& input, char c);
std::vector<std::string>			splitString(const std::string &str, char delimiter);
size_t 								stringToSizeT(const std::string& str);
std::map<std::string, std::string>	create_filetypes();
void								replaceString(std::string &mainString, const std::string &searchString, const std::string &replaceString);
int									count_chars(const std::string &str, char c);
std::string 						getLocalTime();
void								printLog(std::string type ,std::string message);
std::string							decodeURL(const std::string& url);
