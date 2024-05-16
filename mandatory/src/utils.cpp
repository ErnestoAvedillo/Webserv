#include "../inc/utils.hpp"
#include <iostream>
#include <netinet/in.h>

// bool    isnumeric()
// {

// }


// bool    isnumeric(std::string ip)
// {

// }

template <typename T>
bool isrange(T value, T min, T max)
{
    if (value < min || value > max)
        return false;
    return true;
}

bool validIPAddress(std::string ip)
{
	
}

bool isNumber(std::string str)
{
	int i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	for ( ; str[i]; i++)
	{
		if (std::isdigit(str[i]) == 0) 
			return false;
	}
	return true;
}

int main()
{
    std::string nbr = "+29020";
  
    std::cout << std::boolalpha << isNumber(nbr) << std::endl;
	std::cout << std::boolalpha << isrange(1000, IPPORT_RESERVED, 46555) << std::endl ;
}
