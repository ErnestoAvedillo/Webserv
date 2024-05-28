#include "../inc/ListeningSocket.hpp"

#ifdef __APPLE__

ListeningSocket::ListeningSocket(int myPort, Server *srv)
{
	this->port = myPort;
	this->server = srv;
	this->first_read = true;
	this->client = new Client(srv);
	this->receiver = new Receive();
	this->socketFd = -1;
	this->startListening();

}
ListeningSocket::ListeningSocket(Server *srv)
{
	this->client = new Client(srv);
	this->receiver = new Receive();
	this->server = srv;
	this->first_read = true;
}

ListeningSocket::~ListeningSocket()
{
	stopListening();
	delete this->client;
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
		std::cerr << "Failed to bind socket to address of port " << port << std::endl;
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
	socketFd = -1;
	this->startListening();
}
ListeningSocket::ListeningSocket(Server *srv)
{
	this->server = srv;
	this->client = new Client(srv);
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
	if ((send(clientSocketFd, answer.c_str(), answer.size(), 0)) < 0)
	{
		std::cerr << "Failed to write to client" << std::endl;
	}
	return this->client->isSendComplete();
}

bool ListeningSocket::receive(size_t size)
{
	// if (this->first_read)
	// {
	// 	this->first_read = false;
	setSize(size);
	// }
	bool ret = receiver->receive(this->socketFd);
	std::cout << std::boolalpha << "BOOOLALLPHA recieve" << ret << std::endl;
	// if (ret && size == 0)
	// 	return true;
	// else
	// 	return false;
	return(ret);
}

ListeningSocket *ListeningSocket::clone(int fd)
{
	ListeningSocket *newSocket = new ListeningSocket(this->server);
	newSocket->socketFd = fd;
	this->first_read = true;
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
	std::cout << "request by receiver: " << receiver->getRequest() << std::endl;
	std::cout << "body by receiver: $" ;
	// print_visible(receiver->getBody());
	// std::cout << "$" << std::endl;
	if (receiver->getRequest().find("POST") != std::string::npos)
	{
		std::fstream file("./file.png", std::ios::out | std::ios::binary | std::ios::app);
		std::string rec = receiver->getBody().substr(receiver->getBody().find("\r\n\r\n") + 4);
		std::string header = receiver->getBody().substr(0, receiver->getBody().find("\r\n\r\n") + 4);

		// std::cout << "header: " << header << std::endl;
		std::string key_file = header.substr(header.find_first_of("-"), header.find("\r\n"));
		// std::cout << "header: " << header << std::endl;
		rec = rec.substr(0, rec.find(key_file) - 2);

		// std::cout << "rec: " << rec << std::endl;
		file.write(rec.c_str(), rec.size());
	}
	this->client->loadCompleteClient(receiver->getRequest());
}

std::string ListeningSocket::getServerName()
{
	return server->getServerName();
}

Client *ListeningSocket::getClientPtr()
{
	return this->client;
}

void ListeningSocket::setSize(size_t size)
{
	this->receiver->setSize(size);
}
