#pragma once
# include <iostream>
# include <string>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include "../inc/utils.hpp"
# include <iostream>
# include <cstring> // Required for memset


class Parser
{
	private:
	public:
		static bool checkPorts(std::vector<std::string> const &ports);
		static bool checkPort(std::string port);
		static bool checkHost(std::string host);
		static bool checkServerName(std::string serverName);
		static bool checkErrorPage(std::string errorPage);
		static bool checkRoot(std::string root);
		static bool checkIndex(std::string index, std::string root);
		static in_addr_t	isValidHost(std::string hostname);
};