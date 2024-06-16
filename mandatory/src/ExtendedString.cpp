#include "../inc/ExtendedString.hpp"

ExtendedString::ExtendedString() : std::string() {}

ExtendedString::ExtendedString(const std::string &str) : std::string(str) {}

ExtendedString::~ExtendedString() {}

ExtendedString &ExtendedString::operator=(const std::string &str)
{
	std::string::operator=(str);
	return *this;
}

bool ExtendedString::isNumber() const {
    size_t position = 0;
    if (this->empty())
        return false;
    if (this->firstCharIsSign())
        position++;
    for (; position < this->size(); position++) {
        if (!std::isdigit(this->at(position)))
            return false;
    }
    return true;
}

bool ExtendedString::firstCharIsSign() const {
    return this->at(0) == '-' || this->at(0) == '+';
}

void ExtendedString::replaceString(const std::string& oldStr, const std::string& newStr) {
    size_t pos = 0;
    while ((pos = this->find(oldStr, pos)) != std::string::npos) {
        this->replace(pos, oldStr.length(), newStr);
        pos += newStr.length();
    }
}

bool ExtendedString::validIPAddress()
{
	size_t pos = 0;
	ExtendedString token;
	int count = 0;
	
	while ((pos = this->find('.')) != std::string::npos)
	{
		token = this->substr(0, pos);
		if (!token.isNumber() || !isrange(std::atoi(token.c_str()), 0, 255))
			return false;
		this->erase(0, pos + 1);
		count++;
	}
	
	if (count != 4 || !this->isNumber() || !isrange(std::atoi(this->c_str()), 0, 255))
		return false;
	
	return count == 4;
}

size_t ExtendedString::stringToSizeT() {
	std::stringstream ss(*this);
	size_t result = 0;
	ss >> result;
	return result;
}

std::vector<ExtendedString> ExtendedString::splitString(char delimiter) {
	std::vector<ExtendedString> result;
	ExtendedString token;
	std::istringstream tokenStream(*this);

	while (std::getline(tokenStream, token, delimiter)) {
		// if (token != "")
			result.push_back(token);
	}

	return result;
}

//***To be used in case do not want to modify the original string
// ExtendedString ExtendedString::removeCharFromString(char charToRemove) {
// 	ExtendedString result;
// 	for (size_t i = 0; i < this->size(); i++) {
// 		if (this->at(i) != charToRemove) {
// 			result += this->at(i);
// 		}
// 	}
// 	return result;
// }

void ExtendedString::removeCharFromString(char charToRemove) {
    size_t pos = 0;
    while ((pos = this->find(charToRemove, pos)) != std::string::npos) {
        this->erase(pos, 1);
    }
}

//***To be used in case do not want to modify the original string
// ExtendedString ExtendedString::removeBlanksAndTabs() {
// 	ExtendedString result;
// 	for (size_t i = 0; i < this->size(); i++) {
// 		if (this->at(i) != ' ' && this->at(i) != '\t') {
// 			result += this->at(i);
// 		}
// 	}
// 	return result;
// }

void ExtendedString::removeBlanksAndTabs() {
    size_t pos = 0;
    while ((pos = this->find(' ', pos)) != std::string::npos) {
        this->erase(pos, 1);
    }
    while ((pos = this->find('\t', pos)) != std::string::npos) {
        this->erase(pos, 1);
    }
}

int ExtendedString::count_chars(char c) {
	int count = 0;
	for (size_t i = 0; i < this->size(); i++) {
		if (this->at(i) == c) {
			count++;
		}
	}
	return count;
}
