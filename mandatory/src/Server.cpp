#include "../inc/Server.hpp"

std::map<std::string, int> var_names_server()
{
	std::map<std::string, int> varnames;
	varnames[VAR_PORT] = 0;
	varnames[VAR_HOST] = 0;
	varnames[VAR_SERVER_NAME] = 0;
	varnames[VAR_ERROR_PAGE] = 0;
	varnames[VAR_ROOT] = 0;
	varnames[VAR_INDEX] = 0;
	varnames[VAR_CLIENT_MAX_BODY_SIZE] = 0;
	varnames[VAR_AUTOINDEX] = 0;
	return varnames;
}

std::map<std::string, void (Server::*)(const std::string &)> ServerSetters()
{
	std::map<std::string, void (Server::*)(const std::string &)> serverMethods;

	serverMethods[VAR_PORT] = &Server::setPorts;
	serverMethods[VAR_HOST] = &Server::setHost;
	serverMethods[VAR_SERVER_NAME] = &Server::setServerName;
	serverMethods[VAR_ERROR_PAGE] = &Server::setErrorPage;
	serverMethods[VAR_ROOT] = &Server::setRoot;
	serverMethods[VAR_INDEX] = &Server::setIndex;
	serverMethods[VAR_CLIENT_MAX_BODY_SIZE] = &Server::setMaxClientBodySizeStr;
	serverMethods[VAR_AUTOINDEX] = &Server::setAutoindex;
	return serverMethods;
}

Server::Server(std::string &str)
{
	this->cgiModule = new CGI();
	while (str.find("location") != std::string::npos)
	{
		std::string aux = str.substr(str.find("location:{"));
		std::string location  = aux.substr(10, aux.find("}") - 10);
		str.erase(str.find("location"), aux.find("}") + 1);
		this->addLocation(location);
	}
	if(this->loadData(str) == -1)
	{
		std::cerr << CHR_RED << "Error: No se ha podido cargar la configuración del servidor. Parámetros por defecto establecidos." << RESET << std::endl;
		exit(1);
	}
}

Server::~Server() 
{
	delete cgiModule;
	for (size_t i = 0; i < this->locations.size(); i++)
		delete this->locations[i];
}

Server::Server(Server const &copy) {
	*this = copy;
}

Server &Server::operator=(Server const &copy) {
	if (this != &copy) {
		this->isDefault = copy.isDefault;
		this->port = copy.port;
		this->maxBodySizeStr = copy.maxBodySizeStr;
		this->maxBodySize = copy.maxBodySize;
		this->Host = copy.Host;
		this->serverName = copy.serverName;
		this->errorPage = copy.errorPage;
		this->root = copy.root;
		this->index = copy.index;
	}
	return *this;
}

int Server::loadData(std::string const &content) {
	std::string line;
	std::string straux;
	std::map<std::string, int> varnames = var_names_server();

	std::istringstream fileContentStream(content.substr(8, content.length() - 1));
	while (std::getline(fileContentStream, line,';'))
	{
		line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
		if(line.length() == 0 || line == "}" || line == "{")
			continue;
		std::map<std::string, int>::iterator it = varnames.begin();
		while (it != varnames.end())
		{
			if (line.substr(0 , line.find(":") ) == it->first)
			{
				if (it->second == 1)
					std::cerr << "Error: duplicated variable " << it->first << std::endl;
				it->second = 1;
				break;
			}
			it++;
		}
		if(line.length() == 0 || line == "}" || line == "{" )
			continue;
		else if (it == varnames.end())
			printLog("ERROR", "Unrecognized variable " + line.substr(0, line.find(":")));
		else
		{
			straux = line.substr(line.find(":") + 1, line.size());
			(this->*ServerSetters()[it->first])(straux);
		}
	}
	return 0;
}

CGI *Server::cgiModuleClone()
{
	return this->cgiModule->clone();
}

void	Server::print()
{
	std::cout << "-------------------- Server --------------------" << std::endl;
	std::cout << "Server Name: " << this->serverName << std::endl;
	std::cout << "Host: " << this->Host << std::endl;
	std::cout << "Error Page: " << this->errorPage << std::endl;
	std::cout << "Root: " << this->root << std::endl;
	std::cout << "Index: " << this->index << std::endl;
	std::cout << "Client Max Body Size: " << this->maxBodySizeStr << std::endl;
	std::cout << "Is Default: " << this->isDefault << std::endl;
	for (size_t i = 0; i < this->ports.size(); i++)
		std::cout << "Port: " << this->ports[i] << std::endl;

	for (size_t i = 0; i < this->locations.size(); i++)
		this->locations[i]->print();
	std::cout << "-----------------------------------------------" << std::endl;
}

void Server::createListeningSockets()
{
	ListeningSocket *ls;
	for (size_t i = 0; i < this->ports.size(); i++)
	{
		ls = new ListeningSocket(stringToSizeT(ports[i]), this);
		this->port[ls->getFd()] = ls;
	}
}


void Server::checkVariables()
{
	if (Parser::checkPorts(this->getPorts()) == false)
		exit(1);
	if (Parser::checkHost(this->getHost()) == false && this->getHost() != "0.0.0.0")
		exit(1);
	else
		this->setHostAddr(Parser::isValidHost(this->getHost()));
	if (Parser::checkServerName(this->getServerName()) == false)
		exit(1);
	if (Parser::checkRoot(this->getRoot()) == false)
		exit (1);
	Parser::checkErrorPage(this->getErrorPage());

	Parser::checkIndex(this->getIndex(), this->getRoot());
	this->setMaxClientBodySize(Parser::checkClientBodySize(this->getMaxClientBodySizeStr()));
	this->autoIndex = Parser::checkAutoIndex(this->autoIndexStr);
	if (this->autoIndex == true && !this->index.empty())
		printLog("WARNING", "autoindex\t\twon't be used because index file is set");
	
	if (this->locations.size())
		std::cout << std::endl;
	for (size_t i = 0; i < this->locations.size(); i++)
	{
		std::cout << CHR_MGENTA"---------Location [" << i + 1 << "]---------" RESET << std::endl;
		this->locations[i]->checkVariables(this->autoIndex);
		std::cout << std::endl;
		printLog("NOTICE", "OK! Location " + toString(i + 1));
		std::cout << CHR_MGENTA"------------------------------" RESET << std::endl;
	}
	
	std::cout << std::endl;
}