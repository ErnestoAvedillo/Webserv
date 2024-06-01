#include <string>

std::string removeCharFromString(const std::string& input, char c) {
	std::string result;
	for (size_t i = 0; i < input.size(); i++) {
		if (input[i] != c) {
			result += input[i];
		}
	}
	return result;
}
