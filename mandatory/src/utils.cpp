#include "../inc/utils.hpp"

template <typename T>
bool isrange(T value, T min, T max)
{
    if (value < min || value > max)
        return false;
    return true;
}

bool isNumber(std::string str)
{
	int i = 0;
	if (str.empty())
		return false;
	if (str[i] == '-' || str[i] == '+')
		i++;
	for ( ; str[i]; i++)
	{
		if (std::isdigit(str[i]) == 0) 
			return false;
	}
	return true;
}

bool validIPAddress(std::string ip)
{
	size_t pos = 0;
	std::string token;
	int count = 0;
	
	while ((pos = ip.find('.')) != std::string::npos)
	{
		token = ip.substr(0, pos);
		if (!isNumber(token) || !isrange(std::atoi(token.c_str()), 0, 255))
			return false;
		ip.erase(0, pos + 1);
		count++;
	}
	
	if (count != 4 || !isNumber(ip) || !isrange(std::atoi(ip.c_str()), 0, 255))
		return false;
	
	return count == 4;
}


bool isDirPermissions(std::string path, int mode)
{
	if (path.empty())
		return false;
	struct stat buffer;
	if (stat(path.c_str(), &buffer) == -1)
		return false;
	if (!S_ISDIR(buffer.st_mode))
		return false;
	if (access(path.c_str(), mode) != 0)
		return false;
	return true;
}

bool isFilePermissions(std::string path, int mode)
{
	if (path.empty())
		return false;
	struct stat buffer;
	if (stat(path.c_str(), &buffer) == -1)
		return false;
	if (!S_ISREG(buffer.st_mode))
		return false;
	if (access(path.c_str(), mode) != 0)
		return false;
	return true;
}

size_t stringToSizeT(const std::string& str) {
	std::stringstream ss(str);
	size_t result = 0;
	ss >> result;
	return result;
}


std::vector<std::string> splitString(const std::string& str, char delimiter) {
	std::vector<std::string> result;
	std::string token;
	std::istringstream tokenStream(str);

	while (std::getline(tokenStream, token, delimiter)) {
		// if (token != "")
			result.push_back(token);
	}

	return result;
}

std::string removeCharFromString(const std::string& input, char c) {
	std::string result;
	for (size_t i = 0; i < input.size(); i++) {
		if (input[i] != c) {
			result += input[i];
		}
	}
	return result;
}

std::string removeBlanksAndTabs(const std::string& input) {
	std::string result;
	for (size_t i = 0; i < input.size(); i++) {
		if (input[i] != ' ' && input[i] != '\t') {
			result += input[i];
		}
	}
	return result;
}

template <typename T>
std::string itos (T n)
{
	std::string str;
	std::stringstream ss;
	
	ss << n;
	ss >> str;

	return  str;
}

void replaceString(std::string& mainString, const std::string& searchString, const std::string& replaceString) {
	size_t pos = 0;
	while ((pos = mainString.find(searchString, pos)) != std::string::npos) {
		mainString.replace(pos, searchString.length(), replaceString);
		pos += replaceString.length();
	}
}

int count_chars(const std::string& str, char c) {
	int count = 0;
	for (size_t i = 0; i < str.size(); i++) {
		if (str[i] == c) {
			count++;
		}
	}
	return count;
}

std::string getTime()
{
	std::time_t currentTime = std::time(NULL);

	// Convert the current time to a string in the desired format
	std::tm* timeInfo = std::gmtime(&currentTime);
	char buffer[80];
	std::strftime(buffer, sizeof(buffer), "%A, %d-%b-%y %H:%M:%S GMT", timeInfo);

	// Set the date in the header
	return(buffer);
};

// std::string getLocalTime()
// {
// 	std::time_t currentTime = std::time(NULL);

//     // Convert the current time to a string in the desired format using local time
//     std::tm* timeInfo = std::localtime(&currentTime);
//     char buffer[80];
//     std::strftime(buffer, sizeof(buffer), "%A, %d-%b-%y %H:%M:%S %Z", timeInfo);

//     // Return the formatted date string
//     return std::string(buffer);
// }

std::string getLocalTime()
{
    std::time_t currentTime = std::time(NULL);

    // Convert the current time to a string in the desired format using local time
    std::tm* timeInfo = std::localtime(&currentTime);
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y/%m/%d %H:%M:%S", timeInfo);

    // Return the formatted date string
    return std::string(buffer);
}

void printLog(std::string type ,std::string message)
{
	if (type == "ERROR")
		std::cout << CHR_YELLOW << getLocalTime() << " [" << type << "]" << "\t\t" RESET << message << RESET << std::endl;
	else if (type == "WARNING")
		std::cout << CHR_YELLOW << getLocalTime() << " [" << type << "]" << "\t\t" RESET << message << RESET << std::endl;
	else if (type == "NOTICE")
		std::cout << CHR_GREEN << getLocalTime() << " [" << type << "]" << "\t\t" RESET << message << RESET << std::endl;
	else if (type == "DEBUG")
		std::cout << getLocalTime() << " [" << type << "]" << CHR_BLUE " | " RESET << message << std::endl;
}

template <typename T>
T min(T a, T b)
{
	return a < b ? a : b;
}

template <typename T>
T max(T a, T b)
{
	return a > b ? a : b;
}