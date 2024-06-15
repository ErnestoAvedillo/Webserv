#pragma once
#pragma once
# include <string>
# include <vector>
# include <sstream>
# include <fstream>
# include <iostream>
#ifdef __APPLE__
	# include "Server.hpp"
	# include "Location.hpp"
	# include "ListeningSocket.hpp"

	# include <sys/socket.h>
	# include <netinet/in.h>
	# include <sys/event.h>
#endif
#ifdef __linux__
	# include "Server.hpp"
	# include "Location.hpp"
	# include "ListeningSocket.hpp"

	# include <sys/socket.h>
	# include <netinet/in.h>
	# include <sys/epoll.h>
#endif
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <netdb.h>
# include <arpa/inet.h>
# include "colors.h"
# include "Parser.hpp"

#define BACKLOG 10
#define MAX_CLIENTS 100
#define MAX_EVENTS 200
//  The backlog parameter defines the maximum length for the queue of pending
//      connections.  If a connection request arrives with the queue full, the
//      client may receive an error with an indication of ECONNREFUSED


class WebServer {
	private:
		std::ifstream		configFile;
		std::string			configFilename;
		std::string 		configFileString;
		/* Socket Configuration */
		std::map<int, ListeningSocket *>	serverSocket;
		std::map<int, ListeningSocket *>	acceptedSocket;
		
		std::vector<Server *>	servers;
		std::map<std::string, std::string>	mimeTypes;
		std::map<int, std::string>	errorPages;
		int kq;

	public:
		static bool ExitFlag;
		WebServer();
		~WebServer();
		WebServer(WebServer const &copy);
		WebServer &operator=(WebServer const &copy);
	
		void createListeningSockets();
		bool	checkSyntax();
		bool	parseInfo();
		void	processConfigFile();
		void	loadConfigFile(std::string configFile);
		void	launchServers();
		void	eventLoop();
		struct sockaddr_in convertHost(std::string hostname, int port);
		void removeEventFd(int fd, int type);
		void addEvent(int fd, int type);
		void addEventSet();
		void createQueue ();
		int acceptNewEvent(int curfd);
		static void exit_handler(int signum);
		bool checkVariables(Server *server);
		void createServerSocket();
#ifdef __APPLE__
			int waitEvent(struct kevent *evList);
			void modifEvent(struct kevent eventList, int typeRem, int typeAdd);
		#elif __linux__
			int waitEvent(struct epoll_event *evList);
			void modifEvent(struct epoll_event eventList, int type);
		#endif
};
