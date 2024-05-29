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
#include "Client.hpp"
#include "Server.hpp"
#include "Receive.hpp"


#define MAX_CONNECTIONS 10

class Server;

class Client;

class Receive;

// #define MAX_EVENTS 10
class ListeningSocket
{
private:
	Server *server;
	int port;
	int socketFd;
	bool first_read;
	// char *buffer;
	Client *client;
	Receive *receiver;

	//void handleConnection(int clientSocketFd);

public:
	ListeningSocket(int port, Server *srv);
	ListeningSocket(Server *srv);
	~ListeningSocket();

	bool startListening();
	void stopListening();
	int getPort();
	int getFd();
	bool sendData(int);
	void loadRequest();
	std::string getServerName();
	ListeningSocket *clone(int fd);
	Client *getClientPtr();
	bool receive();
	void setSize(size_t size);
};
