#include "../inc/ListeningSocket.hpp"

#ifdef LNX
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

	// Set up the server address
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(port);
	// Bind the socket to the server address
	if (bind(socketFd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
	{
		std::cerr << "Failed to bind socket to address" << std::endl;
		return false;
	}

	// Start listening for incoming connections
	if (listen(socketFd, MAX_CONNECTIONS) < 0)
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
		std::cerr << "Error" << std::endl;
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

void ListeningSocket::sendData(int clientSocketFd)
{
	std::cout << "sendData " << std::endl;
	// std::string buffer = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello, MY World!</h1></body></html>\r\n";
	std::string answer = this->client.getAnswerToSend();
	n = send(clientSocketFd, answer.c_str(), answer.size(), 0);
	if (n < 0)
	{
		std::cerr << "Failed to write to client" << std::endl;
	}
	else
	{
		// std::cout << "Sent " << n << " bytes: " << answer << std::endl;
	}
}

ListeningSocket *ListeningSocket::clone()
{
	ListeningSocket *newSocket = new ListeningSocket(this->server);
	newSocket->socketFd = this->socketFd;
	newSocket->n = this->n;

	return newSocket;
}

void ListeningSocket::loadRequest(char *buff)
{
	this->client.loadCompleteClient(buff);
}