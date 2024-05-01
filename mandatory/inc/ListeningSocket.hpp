#pragma once
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>

#ifdef LNX
	#include </usr/include/sys/socket.h>
	#include </usr/include/sys/epoll.h>
	#include </usr/include/sys/time.h>
	#include </usr/include/sys/types.h>
	//#include </usr/include/sys/event2/event-config.h>
#else
	#include <sys/socket.h>
	#include <sys/event.h>
#endif

#include <unistd.h>
#include <map>
#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CONNECTIONS 10
#define MAX_EVENTS 10
	class ListeningSocket
	{
	public:
		ListeningSocket(int port);
		bool startListening();
		void stopListening();
		void handleEvents();

	private:
		int port;
		int socketFd;
	#ifdef LNX
		int epollFd;
	#else
		int kq;
	#endif
	char buffer[1024];
	int n;

	void handleConnection(int clientSocketFd);
};
