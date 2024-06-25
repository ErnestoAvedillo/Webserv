#pragma once
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>
#include "utils.hpp"

class ExtendedString : public std::string {
public:
	//using std::string::string; // Inherit constructors from std::string
	ExtendedString();
	ExtendedString(const std::string &str);
	~ExtendedString();
	ExtendedString &operator=(const std::string &str);

	bool isNumber() const;
	bool firstCharIsSign() const;
	void replaceString(const std::string& , const std::string& );
	bool validIPAddress();
	size_t stringToSizeT();
	std::vector<ExtendedString> splitString(char );
	//***To be used in case do not want to modify the original string
	//ExtendedString removeCharFromString(char);
	void removeCharFromString(char);
	//***To be used in case do not want to modify the original string
	// ExtendedString removeBlanksAndTabs() ;
	void removeBlanksAndTabs() ;
	int count_chars(char c);
	void replaceFirstString(const std::string& searchString, const std::string& replaceString);

};