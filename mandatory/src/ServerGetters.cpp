#include "../inc/Server.hpp"


size_t Server::getMaxClientBodySize() { return this->maxBodySize; }
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
	// for (std::map<int, ListeningSocket *>::iterator it = this->port.begin(); it != this->port.end(); ++it)
	// {
	// 	std::cout << "Listening on port: " << it->first << " with fd " << it->second->getFd() << std::endl;
	// }
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
