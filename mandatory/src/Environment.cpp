#include "../inc/Environment.hpp"
#include <cstring>

Environment::Environment()
{
}

Environment::~Environment()
{
    std::vector<char*>::iterator itb = this->_envArray.begin();
    std::vector<char*>::iterator ite = this->_envArray.end();  
    while(itb != ite)
    {
        delete *itb;
        itb++;
    }
}

void Environment::setEnv(const std::string key, const std::string value)
{
    this->_env[key] = value;
}

std::vector<char*> Environment::getEnv()
{
    std::map<std::string, std::string>::iterator itb = this->_env.begin();
    std::map<std::string, std::string>::iterator ite = this->_env.end();
    while(itb != ite)
    {
        std::string *tmp = new std::string(itb->first + "=" + itb->second + '\0');
        _envArray.push_back(const_cast<char*>(tmp->c_str()));
        itb++;
    }
    _envArray.push_back(NULL);

    return _envArray;

}

std::string Environment::getEnv(std::string key)
{
    return this->_env[key];
}
