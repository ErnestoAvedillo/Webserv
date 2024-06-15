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
	{
		std::cerr << gai_strerror(status) << std::endl;
		return 0;
	}
	addr = (struct sockaddr_in *)res->ai_addr;
	in_addr = addr->sin_addr.s_addr;

	// printf("IP address in in_addr_t (network byte order): %u\n", in_addr);
	// printf("IP address in in_addr_t (host byte order): %u\n", ntohl(in_addr));

	freeaddrinfo(res);
	return in_addr;
}

bool Parser::checkHost(std::string host)
{
	if (host.length() == 0)
	{
		std::cerr << "Error: Host not defined" << std::endl;
		return false;
	}
	if (!isValidHost(host) && host != "0.0.0.0")
	{
		std::cerr << "Error: invalid host <" << host << ">" << std::endl;
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

	switch (isFilePermissions(root + "/" + index, R_OK))
	{
		case -1:
			printLog("WARNING", "index\t\t<" + index + ">\terror getting file metadata" );
			return false;
		case -3:
			printLog("WARNING", "index\t\t<" + index + ">\tnot a regular file" );
			return false;
		case -4:
			printLog("WARNING", "index\t\t<" + index + ">\tpermission denied" );
			return false;
		case -5:
			printLog("WARNING", "index\t\t<" + index + ">\tfile not found." );
			return false;
		default:
			break ;
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


bool Parser::checkAutoIndex(std::string autoindex)
{
	if (autoindex.length() == 0)
		return false;
	if (autoindex != "on" && autoindex != "off")
	{
		printLog("WARNING", "autoindex\t<" + autoindex + ">\t\tinvalid value. Set to default " CHR_GREEN "off" RESET " value" );
		return false;
	}
	if (autoindex == "on")
		return true;
	else if (autoindex == "off")
		return false;
	return false;
}

int Parser::checkLocationName(std::string name)
{
	if (name.length() == 0)
	{
		printLog("ERROR", "location name\t\t\t\tnot defined." );
		exit(1);
	}
	// if (name == "/")
	// {
	// 	printLog("ERROR", "location name\t\t\t\tcannot be root." );
	// 	return 0;
	// }
	if (name == "cgi-bin")
		return 2;
	return 1;
}

int Parser::checkRootAliasReturn(std::string root, std::string alias, std::string return_)
{
	if (root.empty() && return_.empty() && alias.empty())
	{
		printLog("ERROR", "At least one of root, return or alias must be specified");
		exit(1);
	}
	if ((!root.empty() && !return_.empty()) || (!root.empty() && !alias.empty()) || (!return_.empty() && !alias.empty()))
	{
		printLog("ERROR", "Only one of root, return or alias can be specified");
		exit(1);
	}
	if (!root.empty())
	{
		if (!isDirPermissions(root, R_OK))
		{
			printLog("ERROR", "root\t\t<" + root + ">\tnot a valid directory." );
			exit(1);
		}
		return (ROOT);
	}
	else if (!alias.empty())
	{
		if (!isDirPermissions(alias, R_OK))
		{
			printLog("ERROR", "alias\t\t<" + alias + ">\tnot a valid directory." );
			exit(1);
		}
		return (ALIAS);
	}
	else if (!return_.empty())
	{
		// if (!isDirPermissions(return_, F_OK ) && (return_.find("http://") == std::string::npos && return_.find("https://") == std::string::npos))
		// {
		// 	printLog("ERROR", "return\t\t<" + return_ + ">\tnot a valid directory." );
		// 	exit(1);
		// }
		return (RETURN);
	}
	return (0);
}

bool Parser::checkReturnIgnore(std::string allowMethods, std::string autoindex, std::string index)
{
	if (!allowMethods.empty())
		printLog("WARNING", "allowed_methods\t\twill be ignored when " CHR_GREEN  "return" RESET " is defined.");
	if (!autoindex.empty())
		printLog("WARNING", "autoindex\t\twill be ignored when " CHR_GREEN  "return" RESET " is defined.");
	if (!index.empty())
		printLog("WARNING", "index\t\t\twill be ignored when " CHR_GREEN  "return" RESET " is defined.");
	return true;
}

bool Parser::checkCgiString(std::string cgiPath, std::string cgiExtension)
{
	if (cgiPath.empty() && cgiExtension.empty())
	{
		printLog("ERROR", "cgi_path\t\t\tis not defined.");
		printLog("ERROR", "cgi_extension\t\t\tis not defined.");
		return false;
	}
	else if (cgiPath.empty())
	{
		printLog("ERROR", "cgi_path\t\t\tis not defined.");
		return false;
	}
	else if (cgiExtension.empty())
	{
		printLog("ERROR", "cgi_extension\t\t\tis not defined.");
		return false;
	}
	
	return true;
}

bool Parser::checkCgi(std::vector<std::string> paths, std::vector<std::string> extensions)
{
	if (paths.size() != extensions.size())
	{
		printLog("ERROR", "cgi_path and cgi_extension must have the same number of elements.");
		return false;
	}
	for (size_t size = 0; size < paths.size(); size++)
	{
		switch (isFilePermissions(paths[size], X_OK))
		{
			case -2:
				printLog("ERROR", "cgi_path\t\t<" + paths[size]  + ">\tempty path");
				return false;
			case -1:
				printLog("ERROR", "cgi_path\t\t<" + paths[size] + ">\terror getting file metadata");
				return false;
			case -3:
				printLog("ERROR", "cgi_path\t\t<" + paths[size] + ">\tnot a regular file");
				return false;
			case -4:
				printLog("ERROR", "cgi_path\t\t<" + paths[size] + ">\tpermission denied");
				return false;
			case -5:
				printLog("ERROR", "cgi_path\t\t<" + paths[size] + ">\tfile not found");
				return false;
			default:
				break ;
		}
	}
	// for (size_t size = 0; size < extensions.size(); size++)
	// {
	// 	if (extensions[size].find('.') == std::string::npos)
	// 	{
	// 		printLog("ERROR", "cgi_extension\t\t<" + extensions[size] + ">\tis not a valid extension" );
	// 		return false;
	// 	}
	// }
	return true;
}

bool Parser::checkAllowedMethods(std::string allowMethods)
{
	if (allowMethods.length() == 0)
	{
		printLog("WARNING", "allow_methods\t\tis not defined. Set to default " CHR_GREEN "GET" RESET " value" );
		return false;
	}
	return true;
}