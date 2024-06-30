/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 13:48:33 by eavedill          #+#    #+#             */
/*   Updated: 2024/06/30 14:46:10 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
#define MAX_CONNECTIONS 10

class Server;

class Receive;

class ListeningSocket : public FileContent
{
private:
	Server *server;
	int port;
	int socketFd;
	Receive *receiver;
	Header response;
	Header request;

public:
	ListeningSocket(int port, Server *srv);
	ListeningSocket(Server *srv);
	~ListeningSocket();

	bool			startListening();
	void			stopListening();
	int				getFd();
	bool			sendData(int);
	void			loadRequest(std::vector<Server *> servers);
	ListeningSocket	*clone(int fd);
	std::string		getAnswerToSend();
	bool			receive();
	void			matchServerName(std::vector<Server *> servers);
	void			setCgiEnviroment();
};
