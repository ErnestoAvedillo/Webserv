#pragma once
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "ExtendedString.hpp"

#define SCRIPT_FILENAME_KEY					"SCRIPT_FILENAME"
#define QUERY_STRING_KEY					"QUERY_STRING"
#define REQUEST_METHOD_KEY					"REQUEST_METHOD"
#define CONTENT_TYPE_KEY					"CONTENT_TYPE"
#define CONTENT_LENGTH_KEY					"CONTENT_LENGTH"
#define GATEWAY_INTERFACE_KEY				"GATEWAY_INTERFACE"
#define SERVER_SOFTWARE_KEY					"SERVER_SOFTWARE"
#define REMOTE_ADDR_KEY						"REMOTE_ADDR"
#define REMOTE_PORT_KEY						"REMOTE_PORT"
#define SERVER_ADDR_KEY						"SERVER_ADDR"
#define SERVER_PORT_KEY						"SERVER_PORT"
#define CONTENT_LENGTH_KEY					"CONTENT_LENGTH"
#define CONTENT_TYPE_KEY					"CONTENT_TYPE"
#define SERVER_NAME_KEY						"SERVER_NAME"
#define HTTP_HOST_KEY						"HTTP_HOST"
#define HTTP_USER_AGENT_KEY					"HTTP_USER_AGENT"
#define HTTP_ACCEPT_KEY						"HTTP_ACCEPT"
#define HTTP_ACCEPT_LANGUAGE_KEY			"HTTP_ACCEPT_LANGUAGE"
#define HTTP_ACCEPT_ENCODING_KEY			"HTTP_ACCEPT_ENCODING"
#define HTTP_COOKIE_KEY						"HTTP_COOKIE"
#define HTTP_CONNECTION_KEY					"HTTP_CONNECTION"
#define HTTP_UPGRADE_INSECURE_REQUESTS_KEY	"HTTP_UPGRADE_INSECURE_REQUESTS"

class Environment
{
	private:
		std::map<std::string, std::string> _env;
	public:
		Environment();
		~Environment();
		void setEnv(const std::string $key, const std::string $value);
		char **getEnv();
		std::string getEnv(std::string key);
};
