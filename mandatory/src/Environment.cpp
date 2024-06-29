#include "../inc/Environment.hpp"
#include <cstring>

Environment::Environment()
{
}

Environment::~Environment()
{
}

void Environment::setEnv(const std::string key, const std::string value)
{
    this->_env[key] = value;
}

std::vector<char*> Environment::getEnv()
{
    std::vector<std::string> aux;
    std::vector<char*> env;
    std::map<std::string, std::string>::iterator itb = this->_env.begin();
    std::map<std::string, std::string>::iterator ite = this->_env.end();
    while(itb != ite)
    {
        aux.push_back(itb->first + "=" + itb->second);
        itb++;
    }
    // std::cout<< "-------------------------" << std::endl;
    std::vector<std::string>::iterator itb1 = aux.begin();
    std::vector<std::string>::iterator ite1 = aux.end();
    while(itb1 != ite1)
    {
        // std::cout << itb1->c_str() << std::endl;
        env.push_back(const_cast<char*>(itb1->c_str()));
        itb1++;
    }
    env.push_back(NULL);
 	// 	std::vector<char*>::iterator itb2 = env.begin();
	// 	std::vector<char*>::iterator ite2 = env.end();
	// 	while(itb2 != ite2)
	// 	{
	// 		std::cout << *itb2 << std::endl;
	// 		itb2++;
	// 	}

    // std::cout<< "-------------------------" << std::endl;
    return env;
}

std::string Environment::getEnv(std::string key)
{
    return this->_env[key];
}
