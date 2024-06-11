#include "../inc/Parser.hpp"

bool Parser::checkPorts(std::vector<std::string> const &ports)
{
	if (ports.size() == 0)
	{
		printLog("ERROR", "Port\t\t\t\tnot defined.");
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
		printLog("ERROR", "Port\t\t\t\tnot defined.");
		return false;
	}
	if (!isNumber(port))
	{
		printLog("ERROR", "Port\t\t<" + port + ">\t\tnot a number.");
		return false;
	}
	if (port.length() > 5)
	{
		printLog("ERROR", "Port\t\t<" + port + ">\t\ttoo long.");
		return false;
	}
	
	int MAX_PORT = 65535;
	if (isrange((int)std::atoll(port.c_str()), 0, MAX_PORT) == false)
	{
		printLog("ERROR", "Port\t\t<" + port + ">\t\tout of range.");
		return false;
	}
	return true;
}

in_addr_t Parser::isValidHost(std::string hostname)
{
	struct addrinfo hints, *res;
    struct sockaddr_in *addr;	
	in_addr_t in_addr;

	std::memset(&hints, 0, sizeof(hints));
    std::memset(&in_addr, 0, sizeof(in_addr));
	hints.ai_family = AF_INET; // IPv4
	hints.ai_socktype = SOCK_STREAM; // Or SOCK_DGRAM for UDP	
	hints.ai_flags = AI_PASSIVE;
	
	int status = getaddrinfo(hostname.c_str(), NULL, &hints, &res);
	if (status != 0)
		return 0;
	addr = (struct sockaddr_in *)res->ai_addr;
	in_addr = addr->sin_addr.s_addr;
	freeaddrinfo(res);
	return in_addr;
}

bool Parser::checkHost(std::string host)
{
	if (host.length() == 0)
	{
		printLog("ERROR", "Host \t<" + host + ">\t not defined" );
		return false;
	}
	if (!isValidHost(host) && host != "0.0.0.0")
	{
		printLog("ERROR", "Host \t\t<" + host + ">\tinvalid host" );
		return (false);
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
	if (isDirPermissions(errorPage, R_OK) == false)
	{
		printLog("WARNING", "error_page\t<" + errorPage + ">\tnot a valid directory." );
		return false;
	}
	return true;
}

bool Parser::checkRoot(std::string root)
{
	if (root.length() == 0)
	{
		printLog("ERROR", "root\t\t\t\tnot defined." );
		return false;
	}
	if (isDirPermissions(root, R_OK) == false)
	{
		printLog("ERROR", "root\t\t<" + root + ">\tnot a valid directory." );
		return false;
	}
	return true;
}

bool Parser::checkIndex(std::string index, std::string root)
{
	if (index.length() == 0)
		return true;

	if (isFilePermissions(root + "/" + index, R_OK) == false)
	{
		printLog("WARNING", "index\t\t<" + index + ">\tnot a valid file." );
		return false;
	}
	return true;
}

size_t	Parser::checkClientBodySize(std::string maxClientBodySize)
{
	if (maxClientBodySize.empty())
		return (DEFAULT_MAX_BOD_SIZE);
	if (!isNumber(maxClientBodySize))
	{
		printLog("ERROR", "max_client_body_size\t<" + maxClientBodySize + ">\tis not a number." );
		exit(1);
	}

	int nbr = std::atoi(maxClientBodySize.c_str());
	if (nbr < 0)
	{
		printLog("ERROR", "max_client_body_size\t<" + maxClientBodySize + ">\tcannot be negative." );
		exit(1);
	}
	if (!maxClientBodySize.empty() && nbr == 0)
	{
		printLog("ERROR", "max_client_body_size\t<" + maxClientBodySize + ">\tcannot be zero." );
		exit(1);
	}
	return nbr;
}