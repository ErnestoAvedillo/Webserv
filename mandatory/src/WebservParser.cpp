# include "../inc/WebServer.hpp"

void WebServer::loadConfigFile(std::string configFile) // WebServer loadConfigFile
{
	this->configFilename = configFile;
	this->configFile.open(this->configFilename.c_str());
	if (!this->configFile.is_open())
	{
		std::cerr << "Error: No se ha podido abrir el archivo de configuración" << std::endl;
		exit(1);
	}
	std::stringstream buffer;
	buffer << this->configFile.rdbuf();
	this->fileContent = buffer.str();
	this->processConfigFile();
}

void WebServer::processConfigFile() // WebServer processConfigFile
{
	std::string 		line;
	std::string 		aux;
	std::string 		serverContentConfig = "";
	std::istringstream	fileContentStream(fileContent);
	size_t				pos;

	while (std::getline(fileContentStream, line))
	{
		line = line.substr(0, line.find_first_of("#"));
		if (line.find("/*") != std::string::npos)
		{
			aux = line;
			while (std::getline(fileContentStream, line))
			{
				aux += line;
				if(aux.find("*/") != std::string::npos)
					break;
			}
			if(aux.find("*/") == std::string::npos)
			{
				std::cerr << "Error: Comentario no cerrado" << std::endl;
				exit(1);
			}
			aux.erase(aux.find("/*"), aux.find("*/") + 2);
			line = aux;
		}
		serverContentConfig += line;
	}
	serverContentConfig = removeBlanksAndTabs(serverContentConfig);
	while (serverContentConfig.find("server:{") != std::string::npos)
	{
		pos = serverContentConfig.find("server:{", 8);
		if(pos == std::string::npos)
			aux = serverContentConfig.substr(0, pos);
		else
			aux = serverContentConfig.substr(0, pos - 1);
		// std::cout << "Server to open: " << aux << std::endl;
		servers.push_back(new Server(aux));
		if(serverContentConfig.find("server:{", 8) == std::string::npos)
			break;
		serverContentConfig = serverContentConfig.substr(serverContentConfig.find("server:{", 8), serverContentConfig.length());

	}
}

void WebServer::syntaxServerName(std::string serverName)
{	
	if (serverName.empty())
	{
		std::cerr << "Error: ServerName no puede estar vacío" << std::endl;
		exit(1);
	}
	// return ;
}

void WebServer::syntaxHost(std::string host)
{
	if (host.empty())
	{
		std::cerr << "Error: Host no puede estar vacío" << std::endl;
		exit(1);
	}
	if (!validIPAddress(host))
	{
		std::cerr << "Error: Host no es una dirección IP válida" << std::endl;
		exit(1);
	}
}

void WebServer::syntaxRoot(std::string root)
{
	if (root.empty())
	{
		std::cerr << "Error: Root no puede estar vacío" << std::endl;
		exit(1);
	}
	if (!isDirPermissions(root, R_OK))
	{
		std::cerr << "Error: Root no tiene permisos de lectura" << std::endl;
		exit(1);
	}
}

void WebServer::syntaxIndex(std::string index)
{
	if (index.empty())
	{
		std::cerr << "Error: Index no puede estar vacío" << std::endl;
		exit(1);
	}
}

void WebServer::syntaxErrorPage(std::string errorPage)
{
	if (errorPage.empty())
	{
		std::cerr << "Error: ErrorPage no puede estar vacío" << std::endl;
		exit(1);
	}
}

void WebServer::syntaxClientBodySize(std::string clientBodySize)
{
	if (clientBodySize.empty())
	{
		std::cerr << "Error: ClientBodySize no puede estar vacío" << std::endl;
		exit(1);
	}
	if (!isNumber(clientBodySize))
	{
		std::cerr << "Error: ClientBodySize no es un número" << std::endl;
		exit(1);
	}
}

// void WebServer::syntaxPorts(std::vector<int> ports)
// {
// 	if (ports.empty())
// 	{
// 		std::cerr << "Error: Ports no puede estar vacío" << std::endl;
// 		exit(1);
// 	}
// 	for (size_t i = 0; i < ports.size(); i++)
// 	{
// 		if (!isrange(ports[i], IPPORT_RESERVED, 65535))
// 		{
// 			std::cerr << "Error: Puerto fuera de rango" << std::endl;
// 			exit(1);
// 		}
// 	}
// }

// void WebServer::syntaxLocations(std::vector<Location *> locations)
// {
// 	if (locations.empty())
// 	{
// 		std::cerr << "Error: Locations no puede estar vacío" << std::endl;
// 		exit(1);
// 	}
// }




void WebServer::checkConfig()
{
	for (size_t i = 0; i < servers.size(); i++)
	{
		
		syntaxServerName(servers[i]->getServerName());
		syntaxHost(servers[i]->getHost());
		syntaxRoot(servers[i]->getRoot());
		syntaxIndex(servers[i]->getIndex());
		syntaxErrorPage(servers[i]->getErrorPage());
		
		//syntaxClientBodySize(servers[i]->getClientBodySize());
		// syntaxPorts(servers[i]->getPorts());
		// syntaxLocations(servers[i]->getLocations());

		servers[i]->print();
	}
}
