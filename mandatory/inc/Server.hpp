#ifndef SERVER_HPP
#define SERVER_HPP
# include <vector>
# include <map>
# include <string>
# include <iostream>
# include <sstream>
# include <stdlib.h>
# include "commonFunctions.hpp"

#define VAR_VPORT	"port"
#define VAR_HOST	"host"
#define VAR_SERVER_NAME	"server_name"
#define VAR_ERROR_PAGE	"error_page"
#define VAR_ROOT	"root"
#define VAR_INDEX	"index"


class Location;

class Server {
	protected:
		bool		isDefault;
		std::vector <size_t>	port;
		size_t			maxClientBodySize;
		std::string	Host;
		std::string	serverName;
		std::string errorPage;
		std::string root;
		std::string index;
		//std::vector<class Location> locations;

	public:
		Server();
		Server(std::string const &);
		~Server();
		Server(Server const &);
		Server &operator=(Server const &);
		void	loadData(std::string const &);
		void	setPort(std::string );
		void	setHost(std::string );
		void	setServerName(std::string );
		void	setErrorPage(std::string );
		void	setClientBodySize(std::string );
		void	setRoot(std::string );
		void	setIndex(std::string );
		//void	addLocation(Location );
		void	setIsDefault(std::string );

		size_t		getPort(int i);
		size_t			getClientBodySize();
		bool getIsDefault();
		std::string	getHost();
		std::string	getServerName();
		std::string	getErrorPage();
		std::string	getRoot();
		std::string	getIndex();
		//std::vector<class Location> getLocations();
};

typedef void (Server::*server)(std::string);
typedef std::map<std::string, server> server_methods_t;

#endif
