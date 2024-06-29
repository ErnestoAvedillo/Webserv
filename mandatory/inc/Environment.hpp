#pragma once
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "ExtendedString.hpp"

#define KEYS_DEFINED "COMSPEC,DOCUMENT_ROOT,GATEWAY_INTERFACE,\\
                    HTTP_ACCEPT,HTTP_ACCEPT_ENCODING,\\
                    HTTP_ACCEPT_LANGUAGE,HTTP_CONNECTION,HTTP_HOST,\\
                    HTTP_USER_AGENT,PATH,QUERY_STRING,REMOTE_ADDR,\\
                    REMOTE_PORT,REQUEST_METHOD,REQUEST_URI,\\
                    SCRIPT_FILENAME,SCRIPT_NAME,SERVER_ADDR,\\
                    SERVER_ADMIN,SERVER_NAME,SERVER_PORT,\\
                    SERVER_PROTOCOL,SERVER_SIGNATURE,SERVER_SOFTWARE"

class Environment
{
    private:
        std::map<std::string, std::string> _env;
        std::vector<std::string> _envKeys;
        void DefineKeys();
    public:
        Environment();
        ~Environment();
        void setEnv(const std::string $key, const std::string $value);
        char **getEnv();
        std::string getEnv(std::string key);
        std::vector<std::string> getEnvKeys();

};
