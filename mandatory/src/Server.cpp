/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42barcelona>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 14:24:35 by eavedill          #+#    #+#             */
/*   Updated: 2024/05/27 23:08:30 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	varnames[VAR_CGI_EXTENSION] = 0;
	varnames[VAR_CGI_FOLDER] = 0;
	varnames[VAR_CLIENT_MAX_BODY_SIZE] = 0;
	varnames[VAR_LOCATIONS] = 0;
	return varnames;
}

std::map<std::string, void (Server::*)(const std::string &)> getServerMethods()
{
	std::map<std::string, void (Server::*)(const std::string &)> serverMethods;

	serverMethods[VAR_PORT] = &Server::setPort;
	serverMethods[VAR_HOST] = &Server::setHost;
	serverMethods[VAR_SERVER_NAME] = &Server::setServerName;
	serverMethods[VAR_ERROR_PAGE] = &Server::setErrorPage;
	serverMethods[VAR_ROOT] = &Server::setRoot;
	serverMethods[VAR_INDEX] = &Server::setIndex;
	serverMethods[VAR_CGI_EXTENSION] = &Server::setCGIExtension;
	serverMethods[VAR_CGI_FOLDER] = &Server::setCGIFolder;
	serverMethods[VAR_CLIENT_MAX_BODY_SIZE] = &Server::setClientMaxBodySize;
	serverMethods[VAR_LOCATIONS] = &Server::addLocation;
	return serverMethods;
}

void	Server::setDefaultData()
{
	this->isDefault = false;
	this->port[443] = new ListeningSocket((size_t)443, this);
	this->maxClientBodySize = 1024;
	this->Host = "DefaultHost";
	this->serverName = "DefaultServer";
	this->errorPage = "/Error";
	this->root = "/";
	this->index = "index.html";
}

Server::Server()
{
	//this->setDefaultData();
	std::map<int, ListeningSocket*>::iterator itb = this->port.begin();
	std::map<int, ListeningSocket*>::iterator ite = this->port.end();
	while (itb != ite){
		itb->second->startListening();
		itb++;
	}
}

Server::Server(std::string const &str) 
{
	//this->setDefaultData();
	if(this->loadData(str) == -1)
	{
		std::cerr << CHR_RED << "Error: No se ha podido cargar la configuración del servidor. Parámetros por defecto establecidos." << RESET << std::endl;
		exit(1);
	}
//	std::map<int, ListeningSocket*>::iterator itb = this->port.begin();
//	std::map<int, ListeningSocket*>::iterator ite = this->port.end();
//	while (itb != ite) {
//		itb->second->startListening();
//		itb++;
//	}
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

int Server::loadData(std::string const &content) {
	std::string line;
	std::string straux;
	std::map<std::string, int> varnames = var_names_server();
	if (std::count(content.begin(), content.end(), '{') - std::count(content.begin(), content.end(), '}') != 0)
	{
		std::cerr << "Error: Llaves no balanceadas" << std::endl;
		return -1;
	}
	if(content.find("server:{") != 0)
	{
		std::cerr << "Error: La configuración del servidor debe empezar con \"server:{\"" << std::endl;
		return -1;
	}
	
	std::istringstream fileContentStream(content.substr(8, content.length() - 1));
	while (std::getline(fileContentStream, line,';'))
	{
		if(line == "}")
			continue;
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
			std::cout << "Error: Variable no reconocida: " << line.substr(0, line.find(":")) << std::endl;
		else
		{
				//std::cout << "Line: " << line << std::endl;
				straux = line.substr(line.find(":") + 1, line.size());
				(this->*getServerMethods()[it->first])(straux);
			// std::cout << "Line: " << line << std::endl;
			// straux = line.substr(line.find(":") + 1, line.size());
			// (this->*getServerMethods()[it->first])(straux);
		}
	}
	return 0;
}

void	Server::print()
{
	std::cout << "-------------------- Server --------------------" << std::endl;
	std::cout << "Server Name: " << this->serverName << std::endl;
	std::cout << "Host: " << this->Host << std::endl;
	std::cout << "Error Page: " << this->errorPage << std::endl;
	std::cout << "Root: " << this->root << std::endl;
	std::cout << "Index: " << this->index << std::endl;
	std::cout << "Client Max Body Size: " << this->maxClientBodySize << std::endl;
	std::cout << "Is Default: " << this->isDefault << std::endl;
	for (size_t i = 0; i < this->ports.size(); i++)
		std::cout << "Port: " << this->ports[i] << std::endl;

	for (size_t i = 0; i < this->locations.size(); i++)
		this->locations[i]->print();
	std::cout << "-----------------------------------------------" << std::endl;
}

