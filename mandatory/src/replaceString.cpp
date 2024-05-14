#include <iostream>
#include <string>

void replaceString(std::string& mainString, const std::string& searchString, const std::string& replaceString) {
	size_t pos = 0;
	while ((pos = mainString.find(searchString, pos)) != std::string::npos) {
		mainString.replace(pos, searchString.length(), replaceString);
		pos += replaceString.length();
	}
}
