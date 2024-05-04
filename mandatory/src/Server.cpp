/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 14:24:35 by eavedill          #+#    #+#             */
/*   Updated: 2024/05/04 14:18:39 by eavedill         ###   ########.fr       */
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
	varnames[VAR_CLIENT_MAX_BODY_SIZE] = 0;
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
	serverMethods[VAR_CLIENT_MAX_BODY_SIZE] = &Server::setClientMaxBodySize;
	return serverMethods;
}

void	Server::setDefaultData()
{
	this->isDefault = false;
	this->port[443] = new ListeningSocket(443, this);
	this->maxClientBodySize = 1024;
	this->Host = "DefaultHost";
	this->serverName = "DefaultServer";
	this->errorPage = "/Error";
	this->root = "/";
	this->index = "index.html";
}


Server::Server() {
	this->setDefaultData();
	std::map<int, ListeningSocket*>::iterator itb = this->port.begin();
	std::map<int, ListeningSocket*>::iterator ite = this->port.end();
	while (itb != ite) {
		if (itb->second->startListening()) {
			itb->second->handleEvents();
		}
		itb->second->stopListening();
		itb++;
	}
}

std::vector<int>	Server::getServerFds()
{
	std::vector<int> fd;
	std::map<int, ListeningSocket*>::iterator itb = this->port.begin();
	std::map<int, ListeningSocket*>::iterator ite = this->port.end();
	while (itb != ite) {
		fd.push_back(itb->second->getFd());
		itb++;
	}
	return fd;
}

Server::Server(std::string const &str) 
{
	bool ret;
	this->setDefaultData();
	if(this->loadData(str) == -1)
	{
		std::cerr << "Error: No se ha podido cargar la configuración del servidor. Parámetros por defecto establecidos." << std::endl;
	}
	std::map<int, ListeningSocket*>::iterator itb = this->port.begin();
	std::map<int, ListeningSocket*>::iterator ite = this->port.end();
	while (itb != ite) {
		ret  = itb->second->startListening();
		std::cout << "Listening on port " << itb->first << "  " << ret << std::endl;
		//std::cin.get();

		// 		itb->second->handleEvents();
	// 	itb->second->stopListening();
	 	itb++;
	}
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
			straux = line.substr(line.find(":") + 1, line.size());
			(this->*getServerMethods()[it->first])(straux);
		}
	}
	return 0;
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
				std::cout << "Error: Puerto mal definido." << std::endl;
				exit(1);
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

void Server::setHost(std::string const &host)
{
	this->Host = host;
}

void Server::setServerName(std::string const &server_name)
{
	this->serverName = server_name;
}

void Server::setErrorPage(std::string const &error_page)
{
	this->errorPage = error_page;
}

void Server::setClientMaxBodySize(std::string const &max_client_body_size)
{
	this->maxClientBodySize = stringToSizeT(max_client_body_size);
}

void Server::setRoot(std::string const &root)
{
	this->root = root;
}

void Server::setIndex(std::string const &index)
{
	this->index = index;
}

void Server::addLocation(std::string const &content) {
	this->locations.push_back(new Location(content));
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

ListeningSocket *Server::getPort(int i) {
	std::map<int, ListeningSocket *>::iterator it = this->port.find(i);
	if (it == this->port.end())
	{
		std::cerr << "Error: Puerto no encontrado." << std::endl;
		exit(1);
	}
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

ListeningSocket *Server::getListening(int i) {
	std::map<int, ListeningSocket *>::iterator it = this->port.find(i);
	if (it == this->port.end())
	{
		std::cerr << "Error: Puerto no encontrado." << std::endl;
		exit(1);
	}
	return this->port[i];
}
