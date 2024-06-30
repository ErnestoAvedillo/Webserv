/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42barcelona>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 13:49:18 by eavedill          #+#    #+#             */
/*   Updated: 2024/06/30 13:49:19 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
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
# include "CGI.hpp"
# include "Parser.hpp"
# include "ExtendedString.hpp"

#define VAR_PORT	"port"
#define VAR_HOST	"host"
#define VAR_SERVER_NAME	"server_name"
#define VAR_ERROR_PAGE	"error_page"
#define VAR_ROOT	"root"
#define VAR_INDEX	"index"
#define VAR_CLIENT_MAX_BODY_SIZE	"client_max_body_size"
#define VAR_LOCATIONS	"location"
#define VAR_AUTOINDEX	"autoindex"

#ifdef __APPLE__
	#define READ_EVENT EVFILT_READ
	#define WRITE_EVENT EVFILT_WRITE
	#define END_EVENT EV_EOF
	#define ERR_EVENT EV_EOF
#else
	#define READ_EVENT EPOLLIN
	#define WRITE_EVENT EPOLLOUT
	#define END_EVENT EPOLLHUP
	#define ERR_EVENT EPOLLERR
#endif

class ListeningSocket;
#include "ListeningSocket.hpp"
class Server 
{
	protected:
		bool		isDefault;
		std::map<int, ListeningSocket *> port;
		std::vector<std::string> ports;
		long long			maxBodySize;
		std::string		maxBodySizeStr;
		std::string	Host;
		std::string	serverName;
		ExtendedString errorPage;
		std::string root;
		std::string index;
		std::string autoIndexStr;
		bool autoIndex;
		std::vector<Location *> locations;
		CGI *cgiModule;
		in_addr_t hostAddr;
		// void	setDefaultData();

	public:
		// Server();
		Server(std::string  &);
		~Server();
		Server(Server const &);
		Server &operator=(Server const &);
		//---- Server Methods ------//
		//---- Server Methods ------//
		int		loadData(std::string const &);
		CGI		*cgiModuleClone();
		//---- Server setters ------//
		void	setHost(std::string const &);
		void	setServerName(std::string const &);
		void	setErrorPage(std::string const &);
		void	setMaxClientBodySizeStr(std::string const &max_client_body_size);
		void	setMaxClientBodySize(size_t const &max_client_body_size);
		void	setRoot(std::string const &);
		void	setIndex(std::string const &);
		void	setCGIExtension(std::string const &);
		void	setCGIFolder(std::string const &);
		void	addLocation(std::string const &);
		void	setIsDefault(std::string const &);
		void	setPorts(std::string const &ports);
		void    setHostAddr(in_addr_t Addr);
		void	setAutoindex(std::string const &);

		//---- Server getters ------//
		ListeningSocket *getListening(int i);
		std::vector<std::string> getPorts();
		std::vector<int> getServerFds();

		ListeningSocket *		getPort(int i);
		bool getIsDefault();
		std::string	getHost();
		std::string	getServerName();
		std::string	getErrorPage();
		std::string	getRoot();
		std::string	getIndex();
		std::string	getCGIExtension(const std::string &);
		std::string	getCGIFolder();
		in_addr_t	getHostAddr();
		std::string	getMaxClientBodySizeStr();
		long long		getMaxClientBodySize();
		std::vector<Location *> getLocations();
		bool getAutoIndex();
		void	print();
		void createListeningSockets();
		void checkVariables();

		//std::vector<class Location> getLocations();
};

typedef void (Server::*server)(std::string);
typedef std::map<std::string, server> server_methods_t;

#endif
