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
#include "Server.hpp"
#include "Receive.hpp"
#include "FileContent.hpp"
#include "Header.hpp"
#include "LocationParser.hpp"
#include "Environment.hpp"
// SCRIPT_FILENAME
// QUERY_STRING
// REQUEST_METHOD
// CONTENT_TYPE

// CONTENT_LENGTH

// GATEWAY_INTERFACE CGI/1.1
// SERVER_SOFTWARE	webservcheelave

// REMOTE_ADDR 127.0.0.1
// REMOTE_PORT 8080

// SERVER_ADDR localhost
// SERVER_PORT 8080	
// The name of the authentication scheme used to protect the servlet. For example, BASIC, SSL, or null if the servlet was not protected.

// CONTENT_LENGTH

// The length of the request body in bytes made available by the input stream or -1 if the length is not known. For HTTP servlets, the value returned is the same as the value of the CGI variable CONTENT_LENGTH.

// CONTENT_TYPE

// The MIME type of the body of the request, or null if the type is not known. For HTTP servlets, the value returned is the same as the value of the CGI variable CONTENT_TYPE.

// SERVER_NAME webserver
// HTTP_HOST
// HTTP_USER_AGENT
// HTTP_ACCEPT
// HTTP_ACCEPT_LANGUAGE
// HTTP_ACCEPT_ENCODING
// HTTP_COOKIE
// HTTP_CONNECTION
// HTTP_UPGRADE_INSECURE_REQUESTS


#define MAX_CONNECTIONS 10

class Server;

class Receive;


// #define MAX_EVENTS 10
class ListeningSocket : public FileContent
{
private:
	Server *server;
	int port;
	int socketFd;
	Receive *receiver;
	Header response;
	Header request;

	//void handleConnection(int clientSocketFd);

public:
	ListeningSocket(int port, Server *srv);
	ListeningSocket(Server *srv);
	~ListeningSocket();

	bool startListening();
	void stopListening();
	int getFd();
	bool sendData(int);
	void loadRequest(std::vector<Server *> servers);
	ListeningSocket *clone(int fd);
	std::string getAnswerToSend();
	bool receive();
	void matchServerName(std::vector<Server *> servers);
	void setCgiEnviroment();
};
