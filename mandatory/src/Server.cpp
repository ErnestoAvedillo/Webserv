#include "../inc/Server.hpp"

Server::Server()
{
	std::cout << "Server created" << std::endl;
}

Server::~Server()
{
	std::cout << "Server destroyed" << std::endl;
}

Server::Server(Server const &copy)
{
	*this = copy;
}

Server &Server::operator=(Server const &copy)
{
	if (this != &copy)
	{
		this->isDefault = copy.isDefault;
		this->port = copy.port;
		this->clientBodySize = copy.clientBodySize;
		this->host = copy.host;
		this->serverName = copy.serverName;
		this->errorPage = copy.errorPage;
		this->root = copy.root;
		this->index = copy.index;
		this->locations = copy.locations;
	}
	return *this;
}
