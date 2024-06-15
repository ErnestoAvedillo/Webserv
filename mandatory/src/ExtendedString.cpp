#include "../inc/ExtendedString.hpp"

bool ExtendedString::isNumber() const {
    size_t i = 0;
    if (this->empty())
        return false;
    if (this->at(i) == '-' || this->at(i) == '+')
        i++;
    for (; i < this->size(); i++) {
        if (!std::isdigit(at(i)))
            return false;
    }
    return true;
}

void ExtendedString::substitute(const std::string& oldStr, const std::string& newStr) {
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