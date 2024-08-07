/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 13:51:47 by eavedill          #+#    #+#             */
/*   Updated: 2024/06/30 16:25:05 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ListeningSocket.hpp"

ListeningSocket::ListeningSocket(int myPort, Server *srv): FileContent()
{
	this->port = myPort;
	this->server = srv;
	this->receiver = new Receive();
	this->socketFd = -1;
	if (this->startListening())
	{
		std::string msg = "Listening on " + std::string(CHR_YELLOW) + srv->getHost() + RESET + "\t\t" + std::string(CHR_GREEN) + toString(myPort) + RESET + "\t" + CHR_GREEN + toString(this->socketFd);
		printLog("NOTICE", msg);
	}
	this->loadErrorPageFromDir(srv->getErrorPage());
	this->setIsAutoIndex(srv->getAutoIndex());
	this->setIndexName(srv->getIndex());
	this->setHomeFolder(srv->getRoot());
	this->setCGIModule(srv->cgiModuleClone());
}

ListeningSocket::ListeningSocket(Server *srv): FileContent()
{
	this->receiver = new Receive();
	this->server = srv;
	this->loadErrorPageFromDir(srv->getErrorPage());
	this->setIsAutoIndex(srv->getAutoIndex());
	this->setIndexName(srv->getIndex());
	this->setHomeFolder(srv->getRoot());
	this->setCGIModule(srv->cgiModuleClone());
}

ListeningSocket::~ListeningSocket()
{
	stopListening();
	delete this->receiver;
}

bool ListeningSocket::startListening()
{
	socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFd == -1)
	{
		std::cerr << "Failed to create socket" << std::endl;
		return false;
	}
	if (fcntl(socketFd, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cerr << CHR_RED"Error" << std::endl;
		exit(1);
	}
	int enable = 1;
	setsockopt(socketFd, SOL_SOCKET, SO_KEEPALIVE, &enable, sizeof(enable));
	if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
	{
		std::cerr << "setsockopt(SO_REUSEADDR) failed" << std::endl;
		exit(1);
	}
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = this->server->getHostAddr();
	serverAddress.sin_port = htons(port);
	if (bind(socketFd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
	{
		printLog("ERROR", "Failed to bind socket to address of port " CHR_RED + toString(port) + RESET);
		return false;
	}
	if (listen(socketFd, SOMAXCONN) < 0)
	{
		std::cerr << "Failed to start listening of port " << port << std::endl;
		return false;
	}
	return true;
}

void ListeningSocket::stopListening()
{
	if (socketFd != -1)
	{
		close(socketFd);
		socketFd = -1;
	}
}

int ListeningSocket::getFd()
{
	return socketFd;
}

bool ListeningSocket::sendData(int clientSocketFd)
{
	ExtendedString answer = this->getAnswerToSend();
	int ret;
	if ((ret = send(clientSocketFd, answer.c_str(), answer.size(), 0)) < 0)
		std::cerr << "Failed to write to client" << std::endl;
	else if (ret == 0)
		printLog("WARNING", "send 0 bytes");
	return this->getIsSendComplete();
}

bool ListeningSocket::receive()
{
	bool ret = receiver->receive(this->socketFd);
	return(ret);
}

ListeningSocket *ListeningSocket::clone(int fd)
{
	ListeningSocket *newSocket = new ListeningSocket(this->server);
	newSocket->port = this->port;
	newSocket->socketFd = fd;
	return newSocket;
}

std::string ListeningSocket::getAnswerToSend()
{
	std::string answer;
	std::string filePath = this->getFileName();
	std::string file_content = this->getContent();
	if (this->getFirstFragment())
	{
		answer = response.generateHeader() + file_content;
		this->setFirstFragment(false);
	}
	else
		answer = file_content;

	return (answer);
}

void ListeningSocket::matchServerName(std::vector<Server *> servers)
{
	std::string server_name = request.getAttribute("Host").substr(0, request.getAttribute("Host").find(":"));
	if (server_name.empty())
		return ;
	for (size_t i = 0; i < servers.size(); i++)
	{
		if (servers[i]->getServerName() == server_name)
		{
			this->server = servers[i];
			break;
		}
	}
}

void ListeningSocket::setCgiEnviroment()
{
	this->cgiModule->setEnv(SERVER_ADDR_KEY, server->getHost());
	this->cgiModule->setEnv(SERVER_NAME_KEY, server->getServerName());
	this->cgiModule->setEnv(REQUEST_METHOD_KEY, this->request.getMethod());
	this->cgiModule->setEnv(SERVER_PORT_KEY, toString(this->port));
	this->cgiModule->setEnv(CONTENT_LENGTH_KEY, this->request.getAttribute("Content-Lenght"));
	this->cgiModule->setEnv(CONTENT_TYPE_KEY, this->request.getAttribute("Content-Type"));
	this->cgiModule->setEnv(HTTP_USER_AGENT_KEY, this->request.getAttribute("User-Agent"));
	this->cgiModule->setEnv(SERVER_SOFTWARE_KEY, "ws_cheelave/1.0");
	this->cgiModule->setEnv(GATEWAY_INTERFACE_KEY, "CGI/1.1");

}

void ListeningSocket::loadRequest(std::vector<Server *> servers)
{
	this->request = Header(this->receiver->getRequest());
	matchServerName(servers);
	LocationParser Parser(this->request, this->server, this->receiver);	
	this->setCgiEnviroment();
	try 
	{
		 Parser.checks();
	}
	catch (int e)
	{
		this->getFileContentForStatusCode(e);
		this->request = Parser.getRequest();
		this->response = Parser.getResponse();
		return ;
	}

	Parser.setCookies();
	this->request = Parser.getRequest();
	this->cgiModule->setEnv(PATH_INFO_KEY, this->request.getPath().substr(0, this->request.getPath().rfind("/")));
	this->cgiModule->setEnv(SCRIPT_FILENAME_KEY, this->request.getPath());
	this->response = Parser.getResponse();
	this->setIsAutoIndex(Parser.getIsAutoIndex());
	this->setIsCGI(Parser.getIsCGI());
	this->setFileName(this->request.getPath(), Parser.getQuery());
}
