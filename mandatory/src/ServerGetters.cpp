#include "../inc/Server.hpp"

size_t Server::getClientBodySize() { return this->maxClientBodySize; }
bool Server::getIsDefault() { return this->isDefault; }
std::string Server::getHost() { return this->Host; }
std::string Server::getServerName() { return this->serverName; }
std::string Server::getErrorPage() { return this->errorPage; }
std::string Server::getRoot() { return this->root; }
std::string Server::getIndex() { return this->index; }
std::string Server::getCGIExtension(const std::string &str) { return this->cgiExtension[str]; }
std::map <std::string, std::string>::iterator Server::findCGIExtension(const std::string &str) { return this->cgiExtension.find(str);}
std::map <std::string, std::string>::iterator Server::CGIBegin() { return this->cgiExtension.begin();}
std::map <std::string, std::string>::iterator Server::CGIEnd() { return this->cgiExtension.end();}

ListeningSocket *Server::getPort(int i) {
	std::map<int, ListeningSocket *>::iterator it = this->port.find(i);
	if (it == this->port.end())
	{
		std::cerr << "Error: Puerto no encontrado." << std::endl;
		exit(1);
	}
	return this->port[i];
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
	return this->cgiFolder;
}
