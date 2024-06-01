#include <sstream>
#include <string>

size_t stringToSizeT(const std::string& str) {
	std::stringstream ss(str);
	size_t result = 0;
	ss >> result;
	return result;
}
