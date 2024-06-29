#include "../inc/Environment.hpp"

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

char **Environment::getEnv()
{
    std::vector<char*> RetornoEnv;
    char **env;
    std::map<std::string, std::string>::iterator itb = this->_env.begin();
    std::map<std::string, std::string>::iterator ite = this->_env.end();
    while (itb != ite)
    {
        std::string envVar = itb->first + "=" + itb->second;
        RetornoEnv.push_back(const_cast<char*>(envVar.c_str()));
        ++itb;
    }
    RetornoEnv.push_back(NULL);
    env = RetornoEnv.data();
    return env;
}

std::string Environment::getEnv(std::string key)
{
    return this->_env[key];
}
