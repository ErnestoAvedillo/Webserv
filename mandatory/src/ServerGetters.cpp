/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerGetters.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42barcelona>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 13:51:14 by eavedill          #+#    #+#             */
/*   Updated: 2024/06/30 13:51:14 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"


long long Server::getMaxClientBodySize() { return this->maxBodySize; }
std::string Server::getMaxClientBodySizeStr() { return this->maxBodySizeStr; }
bool Server::getIsDefault() { return this->isDefault; }
std::string Server::getHost() { return this->Host; }
std::string Server::getServerName() { return this->serverName; }
std::string Server::getErrorPage() { return this->errorPage; }
std::string Server::getRoot() { return this->root; }
std::string Server::getIndex() { return this->index; }
in_addr_t Server::getHostAddr() {return this->hostAddr; }
std::vector<Location *>Server::getLocations() { return this->locations; }

ListeningSocket *Server::getPort(int i) {
	std::map<int, ListeningSocket *>::iterator it = this->port.find(i);
	if (it == this->port.end())
	{
		std::cerr << "Error: Puerto no encontrado." << std::endl;
		exit(1);
	}
	return this->port[i];
}

std::vector<std::string> Server::getPorts() { return this->ports; }

ListeningSocket *Server::getListening(int i)
{
	std::map<int, ListeningSocket *>::iterator it = this->port.find(i);
	if (it == this->port.end())
	{
		std::cerr << "Error: Puerto no encontrado." << std::endl;
		exit(1);
	}
	return this->port[i];
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

std::string Server::getCGIFolder()
{
	return this->cgiModule->getCGIFolder();
}


bool Server::getAutoIndex()
{
	return this->autoIndex;
}