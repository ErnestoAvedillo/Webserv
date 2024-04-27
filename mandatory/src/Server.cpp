/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 14:24:35 by eavedill          #+#    #+#             */
/*   Updated: 2024/04/27 18:44:13 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

std::map<std::string, int> var_names()
{
	std::map<std::string, int> varnames;
	varnames["port"] = 0;
	varnames["host"] = 0;
	varnames["server_name"] = 0;
	varnames["error_page"] = 0;
	varnames["root"] = 0;
	varnames["index"] = 0;
	varnames["client_body_size"] = 0;
	return varnames;
}

std::map<std::string, server> getServerMethods() {
	server_methods_t serverMethods;
	serverMethods["port"] = &Server::setPort;
	serverMethods["host"] = &Server::setHost;
	serverMethods["server_name"] = &Server::setServerName;
	serverMethods["error_page"] = &Server::setErrorPage;
	serverMethods["root"] = &Server::setRoot;
	serverMethods["index"] = &Server::setIndex;
	serverMethods["client_body_size"] = &Server::setClientBodySize;
	return serverMethods;
}

Server::Server() {
	this->isDefault = false;
	this->port.push_back(0);
	this->maxClientBodySize = 0;
	this->Host = "";
	this->serverName = "";
	this->errorPage = "";
	this->root = "";
	this->index = "";
}

Server::Server(std::string const &str) 
{
	this->loadData(str);
}

Server::~Server() {}

Server::Server(Server const &copy) {
	*this = copy;
}

Server &Server::operator=(Server const &copy) {
	if (this != &copy) {
		this->isDefault = copy.isDefault;
		this->port = copy.port;
		this->maxClientBodySize = copy.maxClientBodySize;
		this->Host = copy.Host;
		this->serverName = copy.serverName;
		this->errorPage = copy.errorPage;
		this->root = copy.root;
		this->index = copy.index;
		//this->locations = copy.locations;
	}
	return *this;
}

void Server::loadData(std::string const &content) {
	std::string line;
	std::string straux;
	std::map<std::string, int> varnames = var_names();
	std::istringstream fileContentStream(content);
	while (std::getline(fileContentStream, line))
	{
		std::map<std::string, int>::iterator it = varnames.begin();
		while (it != varnames.end())
		{
			if (line.find(it->first) != std::string::npos)
			{
				if (it->second == 1)
					std::cout << "Error: " << it->first << " ha sido ya asignado." << std::endl;
				it->second = 1;
				break;
			}
			it++;
		}
		if (it == varnames.end())
			std::cout << "Error: Variable no reconocida." << std::endl;
		else
		{
			straux = line.substr(line.find(":") + 1, line.size());
			(this->*getServerMethods()[it->first])(straux);
		}
	}

}

void Server::setPort(std::string port) {
	std::string aux;
	std::istringstream portStream(port);
	while (std::getline(portStream, aux, ','))
	{	
		if(aux.find(":") != std::string::npos)
		{
			std::vector<std::string>  aux2 = splitString(aux, ':');
			if (aux2.size() != 2)
			{
				std::cout << "Error: Puerto mal definido." << std::endl;
				exit(1);
			}
			for(size_t i = stringToSizeT(aux2[0]); i <= stringToSizeT(aux2[1]); i++)
				this->port.push_back(i);
		}
		else
			this->port.push_back(stringToSizeT(aux));
	}
}

void Server::setHost(std::string host) {
	this->Host = host;
}

void Server::setServerName(std::string server_name) {
	this->serverName = server_name;
}

void Server::setErrorPage(std::string error_page) {
	this->errorPage = error_page;
}

void Server::setClientBodySize(std::string max_client_body_size) {
	this->maxClientBodySize = stringToSizeT(max_client_body_size);
}

void Server::setRoot(std::string root) {
	this->root = root;
}

void Server::setIndex(std::string index) {
	this->index = index;
}

// void Server::addLocation(Location location) {
// 	//this->locations.push_back(location);
// }

void Server::setIsDefault(std::string is_default) {
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

size_t Server::getPort(int i) {
	return this->port[i];
}

size_t Server::getClientBodySize() {
	return this->maxClientBodySize;
}

bool Server::getIsDefault() {
	return this->isDefault;
}

std::string Server::getHost() {
	return this->Host;
}

std::string Server::getServerName() {
	return this->serverName;
}

std::string Server::getErrorPage() {
	return this->errorPage;
}

std::string Server::getRoot() {
	return this->root;
}

std::string Server::getIndex() {
	return this->index;
}
