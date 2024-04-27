#pragma once
# include <vector>
# include <string>
# include <iostream>
# include "Location.hpp"

class Location;

class Server {
	protected:
		bool		isDefault;
		int			port;
		int			clientBodySize;
		std::string	host;
		std::string	serverName;
		std::string errorPage;
		std::string root;
		std::string index;
		std::vector<class Location> locations;

	public:
		Server();
		~Server();
		Server(Server const &copy);
		Server &operator=(Server const &copy);
		Server *loadData();
		void	setPort(int port);
		void	setHost(std::string host);
		void	setServerName(std::string serverName);
		void	setErrorPage(std::string errorPage);
		void	setClientBodySize(int clientBodySize);
		void	setRoot(std::string root);
		void	setIndex(std::string index);
		void	addLocation(Location location);
		void	setIsDefault(bool isDefault);

		int			getPort();
		int			getClientBodySize();
		bool getIsDefault();
		std::string	getHost();
		std::string	getServerName();
		std::string	getErrorPage();
		std::string	getRoot();
		std::string	getIndex();
		std::vector<class Location> getLocations();
};
