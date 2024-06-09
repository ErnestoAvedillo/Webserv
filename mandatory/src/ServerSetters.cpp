#include "../inc/Server.hpp"

void Server::setHost(std::string const &host) { this->Host = host; }
void Server::setServerName(std::string const &server_name) { this->serverName = server_name; }
void Server::setErrorPage(std::string const &error_page) { this->errorPage = error_page; }
void Server::setClientMaxBodySize(std::string const &max_client_body_size) { this->maxClientBodySize = stringToSizeT(max_client_body_size); }
void Server::setRoot(std::string const &root) { this->root = root; }
void Server::setIndex(std::string const &index) { this->index = index; }
void Server::setCGIFolder(std::string const &str) { this->cgiModule->setCGIFolder(str); }
void Server::addLocation(std::string const &content) { this->locations.push_back(new Location(content)); }

void Server::setCGIExtension(std::string const &str) 
{
	this->cgiModule->setCGIMapExtensions(str);
}

void Server::setIsDefault(std::string const &is_default)
{
	if (is_default == "true")
		this->isDefault = true;
	else if (is_default == "false")
		this->isDefault = false;
	else
	{
		std::cerr << "Error: Valor de is_default no reconocido." << std::endl;
		exit(1);
	}
}

void Server::setPort(std::string const &port)
{
	ListeningSocket *ls;
	std::string aux;
	std::istringstream portStream(port);
	if(this->port.size() != 0)
		this->port.clear();
	while (std::getline(portStream, aux, ','))
	{	
		if(aux.find(":") != std::string::npos)
		{
			std::vector<std::string>  aux2 = splitString(aux, ':');
			if (aux2.size() != 2)
			{
				std::cerr << "Error: Puerto mal definido." << std::endl;
				exit(1);
			}
			if(stringToSizeT(aux2[0]) >= stringToSizeT(aux2[1]))
			{
				std::cerr << "Error: puerto " << aux2[0] << "<" << aux[1] << "Definicion de puertos incorrecta." << std::endl;
			}

			for(size_t i = stringToSizeT(aux2[0]); i <= stringToSizeT(aux2[1]); i++)
			{
				ls = new ListeningSocket(i, this);
				this->port[ls->getFd()] = ls;
			}
		}
		else
		{
			ls = new ListeningSocket(stringToSizeT(aux), this);
			this->port[ls->getFd()] = ls;
		}
	}
}


void Server::setAutoindex(std::string const &theAutoIndex)
{
	if (theAutoIndex == "on")
		this->autoIndex = true;
	else if (theAutoIndex == "off")
		this->autoIndex = false;
	else
		throw std::runtime_error("Error: Valor de autoindex no reconocido.");
}

