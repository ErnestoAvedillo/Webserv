#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>
#include "utils.hpp"

class ExtendedString : public std::string {
public:
	//using std::string::string; // Inherit constructors from std::string

	ExtendedString &operator=(const std::string &str) {
		std::string::operator=(str);
		return *this;
	}

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

};