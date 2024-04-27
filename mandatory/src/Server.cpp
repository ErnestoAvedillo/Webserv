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

int Server::getPort()
{
	return this->port;
}

std::string Server::getHost()
{
	return this->host;
}

std::string Server::getServerName()
{
	return this->serverName;
}

std::string Server::getErrorPage()
{
	return this->errorPage;
}

std::string Server::getRoot()
{
	return this->root;
}

std::string Server::getIndex()
{
	return this->index;
}

// std::vector<class Location> Server::getLocations()
// {
// 	return this->locations;
// }

int Server::getClientBodySize()
{
	return this->clientBodySize;
}

bool Server::getIsDefault()
{
	return this->isDefault;
}

void Server::setIsDefault(bool isDefault)
{
	this->isDefault = isDefault;
}

void Server::setClientBodySize(int clientBodySize)
{
	this->clientBodySize = clientBodySize;
}

void Server::setPort(int port)
{
	this->port = port;
}

void Server::setHost(std::string host)
{
	this->host = host;
}

void Server::setServerName(std::string serverName)
{
	this->serverName = serverName;
}

void Server::setErrorPage(std::string errorPage)
{
	this->errorPage = errorPage;
}

void Server::setRoot(std::string root)
{
	this->root = root;
}

void Server::setIndex(std::string index)
{
	this->index = index;
}

// void Server::setLocations(std::map<std::string, Location> locations)
// {
// 	this->locations = locations;
// }

// void Server::addLocation(std::string path, Location location)
// {
// 	this->locations[path] = location;
// }

void Server::printServer()
{
	std::cout << "---------------------------------------"  << std::endl;
	std::cout << "Port: " << this->port << std::endl;
	std::cout << "Host: " << this->host << std::endl;
	std::cout << "ServerName: " << this->serverName << std::endl;
	std::cout << "ErrorPage: " << this->errorPage << std::endl;
	std::cout << "Root: " << this->root << std::endl;
	std::cout << "Index: " << this->index << std::endl;
	// std::cout << "Locations: " << std::endl;
	// for (std::map<std::string, Location>::iterator it = this->locations.begin(); it != this->locations.end(); it++)
	// {
	// 	std::cout << "Path: " << it->first << std::endl;
	// 	it->second.printLocation();
	// }
}
