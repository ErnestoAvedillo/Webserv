#ifndef SERVER_HPP
#define SERVER_HPP
# include <vector>
# include <map>
# include <string>
# include <iostream>
# include <sstream>
# include <stdlib.h>
# include <string>
# include <algorithm>
# include "utils.hpp"
# include "Location.hpp"

#define VAR_PORT	"port"
#define VAR_HOST	"host"
#define VAR_SERVER_NAME	"server_name"
#define VAR_ERROR_PAGE	"error_page"
#define VAR_ROOT	"root"
#define VAR_INDEX	"index"
#define VAR_CLIENT_MAX_BODY_SIZE	"client_max_body_size"
#define VAR_LOCATIONS	"location"

class ListeningSocket;
#include "ListeningSocket.hpp"

class Server {
	protected:
		bool		isDefault;
		std::map <int,ListeningSocket *>	port;
		std::vector<std::string> ports;
		size_t			maxClientBodySize;
		std::string	Host;
		std::string	serverName;
		std::string errorPage;
		std::string root;
		std::string index;
		std::vector<Location *> locations;
		void	setDefaultData();

	public:
		Server();
		Server(std::string const &);
		~Server();
		Server(Server const &);
		Server &operator=(Server const &);
		int		loadData(std::string const &);
		void	setPort(std::string const &);
		void	setHost(std::string const &);
		void	setServerName(std::string const &);
		void	setErrorPage(std::string const &);
		void	setClientMaxBodySize(std::string const &);
		void	setRoot(std::string const &);
		void	setIndex(std::string const &);
		void	addLocation(std::string const &);
		void	setIsDefault(std::string const &);
		ListeningSocket *	getListening(int i);
		std::vector<int>	getServerFds();

		ListeningSocket *		getPort(int i); 
		size_t			getClientBodySize();
		bool getIsDefault();
		std::string	getHost();
		std::string	getServerName();
		std::string	getErrorPage();
		std::string	getRoot();
		std::string	getIndex();

		void	setPorts(std::string const &ports);
		void	print();
		//std::vector<class Location> getLocations();
};

typedef void (Server::*server)(std::string);
typedef std::map<std::string, server> server_methods_t;

#endif
