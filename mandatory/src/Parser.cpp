#include "../inc/Parser.hpp"

bool Parser::checkPorts(std::vector<std::string> const &ports)
{
	if (ports.size() == 0)
	{
		std::cerr << "Error: Ports not defined" << std::endl;
		return false;
	}
	for (size_t i = 0; i < ports.size(); i++)
	{
		if (checkPort(ports[i]) == false)
			return false;
	}
	return true;
}

bool Parser::checkPort(std::string port)
{
	if (port.length() == 0)
	{
		std::cerr << "Error: Port not defined" << std::endl;
		return false;
	}
	if (!isNumber(port))
	{
		std::cerr << "Error: Port \"" << port << "\" not a number." << std::endl;
		return false;
	}
	if (port.length() > 5)
	{
		std::cerr << "Error: Port \"" << port << "\" too long." << std::endl;
		return false;
	}
	
	int MAX_PORT = 65535;
	if (isrange((int)std::atoll(port.c_str()), 0, MAX_PORT) == false)
	{
		std::cerr << "Error: Port \"" << port << "\" out of range." << std::endl;
		return false;
	}
	return true;
}

bool Parser::checkHost(std::string host)
{
	//!!!!!!!!!! Check when is a string if it a defined host name!!!!!!!!!!!!!!
	if (host.length() == 0)
	{
		std::cerr << "Error: Host not defined" << std::endl;
		return false;
	}

	if (validIPAddress(host))
	{
		std::cerr << "Error: Host \"" << host << "\" not a valid IP address." << std::endl;
		return false;
	}
	return true;
}

bool Parser::checkServerName(std::string serverName)
{
	(void)serverName;
	return true;
}

bool Parser::checkErrorPage(std::string errorPage)
{
	(void)errorPage;
	return true;
}

bool Parser::checkRoot(std::string root)
{
	if (root.length() == 0)
	{
		std::cerr << "Error: Root not defined" << std::endl;
		return false;
	}
	if (isDirPermissions(root, R_OK) == false)
	{
		std::cerr << "Error: Root \"" << root << "\" not a valid directory." << std::endl;
		return false;
	}
	return true;
}

bool Parser::checkIndex(std::string index, std::string root)
{
	// if (index.length() == 0)
	// {
	// 	std::cerr << "Error: Index not defined" << std::endl;
	// 	return false;
	// }
	if (index.length() == 0)
		return true;

	if (isFilePermissions(root + "/" + index, R_OK) == false)
	{
		std::cerr << "Error: Index \"" << index << "\" not a valid file." << std::endl;
		return false;
	}
	return true;
}