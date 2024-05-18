#pragma once
# include <string>
# include <vector>
# include <sstream>
# include <fstream>
# include <iostream>

# include "Server.hpp"
# include "Location.hpp"
# include "ListeningSocket.hpp"

# include <sys/socket.h>
# include <netinet/in.h>
# include <sys/event.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <netdb.h>
# include <arpa/inet.h>

#define BACKLOG 20
#define MAX_EVENTS 300
#define MAX_CLIENTS 100
#define MAX_MSG_SIZE 1024
//  The backlog parameter defines the maximum length for the queue of pending
//      connections.  If a connection request arrives with the queue full, the
//      client may receive an error with an indication of ECONNREFUSED

class WebServer {
	private:
		static bool			running;
		std::ifstream		configFile;
		std::string			configFilename;
		std::string 		fileContent;
		/* Socket Configuration */
		std::map<int, ListeningSocket *>	serverSocket;
		std::map<int, ListeningSocket *>	acceptedSocket;
		
		std::vector<Server *>	servers;
		std::map<std::string, std::string>	mimeTypes;
		std::map<int, std::string>	errorPages;
		
		
		int kq;
		int client_events[MAX_CLIENTS];
		void	processConfigFile();

	public:
		WebServer();
		~WebServer();
		WebServer(WebServer const &copy);
		WebServer &operator=(WebServer const &copy);

		void	loadConfigFile(std::string configFile);
		
		void 	checkConfig();
		void	launchServers();
		
		void 	newConnection(int fd);
		void	createSocket();
		void	addEventSet();
		void	eventLoop();
		

		int getConnection(int fd);
		int	addConnection(int fd);
		int	removeConnection(int fd);
		struct sockaddr_in convertHost(std::string hostname, int port);

		void removeFilter(int fd, int type);
		void addFilter(int fd, int type);
		void initalizer();
		static void signalHandler(int signum);

		void syntaxServerName(std::string serverName);
		void syntaxHost(std::string host);
		void syntaxPort(std::string port);
		void syntaxErrorPage(std::string errorPage);
		void syntaxLocation(std::string location);
		void syntaxRoot(std::string root);
		void syntaxIndex(std::string index);
		void syntaxClientBodySize(std::string clientBodySize);

};
