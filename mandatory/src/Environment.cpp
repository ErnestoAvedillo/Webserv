#include "../inc/Environment.hpp"

Environment::Environment()
{
    this->DefineKeys();
}

Environment::~Environment()
{
}

void Environment::DefineKeys()
{
    ExtendedString keys(KEYS_DEFINED);
    std::vector<ExtendedString> _envKeys = keys.splitString(',');
}

void Environment::setEnv(const std::string key, const std::string value)
{
    this->_env[key] = value;
}

char **Environment::getEnv()
{
    std::vector<char*> RetornoEnv;
    int i = 0;
    std::map<std::string, std::string>::iterator itb = this->_env.begin();
    std::map<std::string, std::string>::iterator ite = this->_env.end();
    while (itb != ite)
    {
        std::string envVar = itb->first + "=" + itb->second;
        RetornoEnv.push_back(const_cast<char*>(envVar.c_str()));
        ++itb;
    }
    RetornoEnv.push_back(NULL);
    return RetornoEnv.data();
}

std::string Environment::getEnv(std::string key)
{
    return this->_env[key];
}

std::vector<std::string> Environment::getEnvKeys()
{
    return this->_envKeys;
}