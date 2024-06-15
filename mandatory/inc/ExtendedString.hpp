#include <sstream>
#include <string>
#include <unistd.h>
#include "utils.hpp"

class ExtendedString : public std::string {
public:
	//using std::string::string; // Inherit constructors from std::string

	ExtendedString &operator=(const std::string &str) {
		std::string::operator=(str);
		return *this;
	}
	bool isNumber() const;
	void substitute(const std::string& , const std::string& );
	bool validIPAddress();
	size_t stringToSizeT();

};