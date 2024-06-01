#include "../inc/Server.hpp"

void Server::setHost(std::string const &host) { this->Host = host; }
void Server::setServerName(std::string const &server_name) { this->serverName = server_name; }
void Server::setErrorPage(std::string const &error_page) { this->errorPage = error_page; }
void Server::setClientMaxBodySize(std::string const &max_client_body_size) { this->maxClientBodySize = stringToSizeT(max_client_body_size); }
void Server::setRoot(std::string const &root) { this->root = root; }
void Server::setIndex(std::string const &index) { this->index = index; }
void Server::setCGIExtension(std::string const &cgi_extension) { this->cgiExtension = cgi_extension; }
void Server::setCGIFolder(std::string const &cgi_folder) { this->cgiFolder = cgi_folder; }
void Server::addLocation(std::string const &content) { this->locations.push_back(new Location(content)); }

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

void Server::setPorts(std::string const &ports)
{
	std::string aux;
	std::istringstream portStream(ports);
	if(this->ports.size() != 0)
		this->ports.clear();
	while (std::getline(portStream, aux, ','))
	{
		if (aux.find(":") != std::string::npos)
		{
			std::vector<std::string> aux2 = splitString(aux, ':');
			if (aux2.size() != 2)
			{
				std::cerr << "Error: Wrong set of ports defined" << std::endl;
				exit(1);
			}
			if (isNumber(aux2[0]) == false || isNumber(aux2[1]) == false)
			{
				std::cerr << "Error: Port not a number." << std::endl;
				exit(1);
			}
			for (size_t i = stringToSizeT(aux2[0]); i <= stringToSizeT(aux2[1]); i++)
			{
				this->ports.push_back(toString(i));
			}
			
		}
		else
		{
			if (isNumber(aux) == false)
			{
				std::cerr << "Error: Port not a number." << std::endl;
				exit(1);
			}
			this->ports.push_back(aux);
		}
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
