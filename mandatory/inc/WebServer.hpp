#pragma once
# include <string>
# include <vector>
# include <sstream>
# include <fstream>
# include <iostream>
# include "Server.hpp"
# include "Location.hpp"
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/event.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BACKLOG 10
#define MAX_EVENTS 10
#define MAX_CLIENTS 10
#define MAX_MSG_SIZE 1024
//  The backlog parameter defines the maximum length for the queue of pending
//      connections.  If a connection request arrives with the queue full, the
//      client may receive an error with an indication of ECONNREFUSED
class WebServer {
	private:
		
		std::ifstream		configFile;
		std::string			configFilename;
		std::string 		fileContent;
		/* Socket Configuration */
		std::vector<int>	serverSocket;
		std::vector<int>	serverAccepted;
		int kq;
		int client_events[MAX_CLIENTS] = {0};

	public:
		std::vector<Server>	servers;
		WebServer();
		~WebServer();
		WebServer(WebServer const &copy);
		WebServer &operator=(WebServer const &copy);

		void	loadConfigFile(std::string configFile);
		void	processConfigFile(std::string fileContent);
		
		
		void	launchServers();
		
		
		void	createSocket();
		struct kevent	addEventSet();
		void	eventLoop();
		
		int getConnection(int fd);
		int	addConnection(int fd);
		int	removeConnection(int fd);
};

