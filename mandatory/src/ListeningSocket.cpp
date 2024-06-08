#include "../inc/ListeningSocket.hpp"

#ifdef __APPLE__

ListeningSocket::ListeningSocket(int myPort, Server *srv)
{
	this->port = myPort;
	this->server = srv;
	this->client = new Client(srv);
	this->receiver = new Receive();
	this->socketFd = -1;
	if (this->startListening())
		printLog("NOTICE", "Listening on port: " CHR_GREEN + std::to_string(myPort) + RESET " with file descriptor " CHR_GREEN + std::to_string(this->socketFd));
		// std::cout << CHR_GREEN <<  getLocalTime() << " [NOTICE]" << CHR_BLUE " | " RESET << "Listening on port: " CHR_GREEN<< myPort <<RESET " with file descriptor " << this->socketFd << std::endl;
}
ListeningSocket::ListeningSocket(Server *srv)
{
	this->client = new Client(srv);
	this->receiver = new Receive();
	this->server = srv;
}

ListeningSocket::~ListeningSocket()
{
	stopListening();
	delete this->client;
	delete this->receiver;
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
		printLog("ERROR", "Failed to bind socket to address of port " CHR_RED + std::to_string(port) + RESET);
		return false;
	}

	// Start listening for incoming connections
	if (listen(socketFd, SOMAXCONN) < 0)
	{
		std::cerr << "Failed to start listening of port " << port << std::endl;
		return false;
	}

	// Add the socket file descriptor to the kqueue
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
	this->client = new Client(srv);
	this->receiver = new Receive();
	socketFd = -1;
	this->startListening();
}
ListeningSocket::ListeningSocket(Server *srv)
{
	this->client = new Client(srv);
	this->receiver = new Receive();
	this->server = srv;
}

ListeningSocket::~ListeningSocket()
{
	stopListening();
	delete this->client;
	delete this->receiver;
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
	std::string answer = this->client->getAnswerToSend();
	// std::cout << "answer: " << answer.size() << std::endl;
	// std::cout << "answer: " << answer << std::endl;
	if ((send(clientSocketFd, answer.c_str(), answer.size(), 0)) < 0)
	{
		std::cerr << "Failed to write to client" << std::endl;
	}
	return this->client->isSendComplete();
}

bool ListeningSocket::receive()
{
	bool ret = receiver->receive(this->socketFd);
	return(ret);
}

ListeningSocket *ListeningSocket::clone(int fd)
{
	ListeningSocket *newSocket = new ListeningSocket(this->server);
	newSocket->socketFd = fd;
	return newSocket;
}

// void print_visible(const std::string& str) {
//     for (char ch : str) {
//         switch (ch) {
//             case '\n':
//                 std::cout << "\\n";
//                 break;
//             case '\r':
//                 std::cout << "\\r";
//                 break;
//             case '\t':
//                 std::cout << "\\t";
//                 break;
//             default:
//                 if (std::isprint(static_cast<unsigned char>(ch))) {
//                     std::cout << ch;
//                 } else {
//                     std::cout << "\\x" << std::hex << std::uppercase << static_cast<int>(static_cast<unsigned char>(ch));
//                     std::cout << std::dec;  // Reset to decimal for future use
//                 }
//         }
//     }
// }

void ListeningSocket::loadRequest()
{
	// std::cout << "-------------Request-------------" << std::endl;
	// std::cout << receiver->getRequest() << std::endl;
	this->client->loadCompleteClient(this->receiver);
}

std::string ListeningSocket::getServerName()
{
	return server->getServerName();
}

Client *ListeningSocket::getClientPtr()
{
	return this->client;
}