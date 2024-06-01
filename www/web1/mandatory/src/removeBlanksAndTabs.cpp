#include <string>

std::string removeBlanksAndTabs(const std::string& input) {
	std::string result;
	for (size_t i = 0; i < input.size(); i++) {
		if (input[i] != ' ' && input[i] != '\t') {
			result += input[i];
		}
	}
	return result;
}
