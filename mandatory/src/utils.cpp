#include <iostream>
#include <netinet/in.h>
#include <cstdlib>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream>

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
	
	if (count != 3 || !isNumber(ip) || !isrange(std::atoi(ip.c_str()), 0, 255))
		return false;
	
	return count == 3;
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


// int main()
// {
//     std::string nbr = "+29020";
  
//     std::cout << std::boolalpha << isNumber(nbr) << std::endl;
// 	std::cout << std::boolalpha << isrange(1000, IPPORT_RESERVED, 46555) << std::endl ;
// 	std::cout << std::boolalpha << validIPAddress(".128.255.10.1") << std::endl;
// 	std::cout << std::boolalpha << isDirPermissions("./dir", R_OK | W_OK) << std::endl;
// 	std::cout << std::boolalpha << isFilePermissions("./file_not_read", R_OK | W_OK) << std::endl;
// 	return 0;
// }

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