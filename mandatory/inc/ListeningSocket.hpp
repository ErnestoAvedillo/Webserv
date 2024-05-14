#pragma once
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>

#ifdef __APPLE__
#include <sys/event.h>
#else
#include <sys/epoll.h>
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

class Client;
#include "Client.hpp"
// #define MAX_EVENTS 10
class ListeningSocket
{
private:
	Server *server;
	int port;
	int socketFd;
	int n;
	// char *buffer;
	Client *client;
	//void handleConnection(int clientSocketFd);

public:
	ListeningSocket(int port, Server *srv);
	ListeningSocket(Server *srv);
	~ListeningSocket();

	bool startListening();
	void stopListening();
	int getPort();
	int getFd();
	void sendData(int);
	void loadRequest(char *buff);
	std::string getServerName();

	ListeningSocket *clone();
};
