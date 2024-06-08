#pragma once
# include <iostream>
# include <string>
# include "../inc/utils.hpp"

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
		static bool checkIndex(std::string index);
};