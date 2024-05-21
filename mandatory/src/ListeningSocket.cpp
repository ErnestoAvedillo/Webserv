#include "../inc/ListeningSocket.hpp"

#ifdef __APPLE__

ListeningSocket::ListeningSocket(int myPort, Server *srv)
{
	port = myPort;
	server = srv;
	socketFd = -1;
	this->startListening();
}
ListeningSocket::ListeningSocket(Server *srv)
{
	this->server = srv;
}

ListeningSocket::~ListeningSocket()
{
	stopListening();
}

bool ListeningSocket::startListening()
{
	// Create a socket
	socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFd == -1)
	{
		std::cerr << "Failed to create socket" << std::endl;
		return false;
	}
	std::cout << "created socket nr:" << socketFd << std::endl;

	if (fcntl(socketFd, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cerr << CHR_RED"Error" << std::endl;
		exit(1);
	}

	int enable = 1;
	if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
	{
		std::cerr << "setsockopt(SO_REUSEADDR) failed" << std::endl;
		exit(1);
	}

	// Set up the server address
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(port);

	// Bind the socket to the server address
	if (bind(socketFd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
	{
		std::cerr << "Failed to bind socket to address of port " << port << std::endl;
		return false;
	}

	// Start listening for incoming connections
	if (listen(socketFd, 5) < 0)
	{
		std::cerr << "Failed to start listening of port " << port << std::endl;
		return false;
	}

	// Add the socket file descriptor to the kqueue
	std::cout << "Listening of port " << port << std::endl;
	return true;
}

void ListeningSocket::stopListening()
{
	if (socketFd != -1)
	{
		std::cout << "Closing socket " << socketFd << std::endl;
		close(socketFd);
		socketFd = -1;
	}
}

#else

ListeningSocket::ListeningSocket(int myPort, Server *srv)
{
	port = myPort;
	server = srv;
	socketFd = -1;
	this->startListening();
}
ListeningSocket::ListeningSocket(Server *srv)
{
	this->server = srv;
}

ListeningSocket::~ListeningSocket()
{
	stopListening();
	delete this->client;
}

bool ListeningSocket::startListening()
{
	// Create a socket
	socketFd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (socketFd == -1)
	{
		std::cerr << "Failed to create socket" << std::endl;
		return false;
	}

	if (fcntl(socketFd, F_SETFL, O_NONBLOCK,FD_CLOEXEC) < 0)
	{
		std::cerr << "Error" << std::endl;
		exit(1);
	}

	int enable = 1;
	setsockopt(socketFd, SOL_SOCKET, SO_KEEPALIVE, &enable, sizeof(enable));
	setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));

	// Set up the server address
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(port);
	// Bind the socket to the server address
	if (bind(socketFd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
	{
		std::cerr << "Failed to bind socket to address " << port << std::endl;
		return false;
	}

	// Start listening for incoming connections
	if (listen(socketFd, SOMAXCONN) < 0)
	{
		std::cerr << "Failed to start listening" << std::endl;
		return false;
	}
	std::cout << "Listening on port " << port << std::endl;
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

#endif

int ListeningSocket::getPort()
{
	return (this->port);
}

int ListeningSocket::getFd()
{
	return socketFd;
}

bool ListeningSocket::sendData(int clientSocketFd)
{
	std::cout << "sendData " << std::endl;
	//std::string answer = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello, MY World!</h1></body></html>\r\n";
	std::string answer = this->client->getAnswerToSend();
	//std::cout << "Send " << clientSocketFd << "     " << answer << std::endl;
	n = send(clientSocketFd, answer.c_str(), answer.size(), 0);
	std::cout << "Sent " << n << " bytes to client" << std::endl;
	if (n < 0)
	{
		std::cerr << "Failed to write to client" << std::endl;
	}
	return this->client->isSendComplete();
}

ListeningSocket *ListeningSocket::clone(int fd)
{
	ListeningSocket *newSocket = new ListeningSocket(this->server);
	newSocket->socketFd = fd;
	newSocket->n = this->n;

	return newSocket;
}

void ListeningSocket::loadRequest(char *buff)
{
	this->client = new Client((std::string)buff, server);
	std::cout << "loadRequest created Client " << this->client << std::endl;
	this->client->loadCompleteClient(buff);
}

std::string ListeningSocket::getServerName()
{
	return server->getServerName();
}

Client *ListeningSocket::getClientPtr()
{
	return this->client;
}
