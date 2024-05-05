#pragma once
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
# include <netdb.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>

#ifdef LNX
	#include </usr/include/sys/socket.h>
	#include </usr/include/sys/epoll.h>
	#include </usr/include/sys/time.h>
	#include </usr/include/sys/types.h>
	//#include </usr/include/sys/event2/event-config.h>
#else
# include <sys/socket.h>
# include <sys/types.h>



	#include <sys/event.h>
	#include <sys/time.h>
#endif

#include <unistd.h>
#include <map>
#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CONNECTIONS 10
class Server;
#include "Server.hpp"
// #define MAX_EVENTS 10
	class ListeningSocket
	{
	private:
		Server *server;
		int port;
		int socketFd;
		#ifdef LNX
			int epollFd;
		#else
			int kq;
		#endif
		int n;
		char *buffer;
		
		void handleConnection(int clientSocketFd);

	public:
		ListeningSocket(int port, Server *srv);
		~ListeningSocket();
		bool startListening();
		void stopListening();
		void handleEvents();
		int	getPort();
		int getFd() ;
		void sendData(int);
		ListeningSocket * clone();
		void setBuffer(char *buff);
};
