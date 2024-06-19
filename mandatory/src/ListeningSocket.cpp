#include "../inc/ListeningSocket.hpp"

ListeningSocket::ListeningSocket(int myPort, Server *srv)
{
	this->port = myPort;
	this->server = srv;
	// this->client = new Client(srv);
	this->receiver = new Receive();

	this->socketFd = -1;
	if (this->startListening())
	{
		std::string msg = "Listening on " + std::string(CHR_YELLOW) + srv->getHost() + RESET + "\t\t" + std::string(CHR_GREEN) + std::to_string(myPort) + RESET + "\t" + CHR_GREEN + std::to_string(this->socketFd);
		printLog("NOTICE", msg);
	}
}

ListeningSocket::ListeningSocket(Server *srv)
{
	std::cout << "ListeningSocket::ListeningSocket(Server *srv)" << std::endl;
	// this->client = new Client(srv);
	this->receiver = new Receive();
	this->fileContent = new FileContent(srv);
	this->server = srv;
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

	// Set up the server address
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = this->server->getHostAddr();
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
	std::string answer = this->getAnswerToSend();
	if ((send(clientSocketFd, answer.c_str(), answer.size(), 0)) < 0)
		std::cerr << "Failed to write to client" << std::endl;
	return this->fileContent->isSendComplete();
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

std::string ListeningSocket::getAnswerToSend()
{
	std::string answer;
	std::string filePath = this->fileContent->getFileName();
	std::string file_content = this->fileContent->getContent();
	if (this->fileContent->getFirstFragment())
	{
		if (response.getContentType().find("video/") != std::string::npos && this->request.getAttribute("Sec-Fetch-Dest").find("document") != std::string::npos)
			response.setAttribute("Accept-Ranges", "bytes");
		answer = response.generateHeader() + file_content;
		this->fileContent->setFirstFragment(false);
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

void ListeningSocket::loadRequest(std::vector<Server *> servers)
{
	this->request = Header(this->receiver->getRequest());
	matchServerName(servers);
	LocationParser LocationParser(this->request, this->server, this->receiver);
	this->request = LocationParser.getRequest();
	this->response = LocationParser.getResponse();
	// this->fileContent->setIsAutoIndex(this->server->getAutoIndex());
	// this->fileContent->setIsCGI(this->server->getCGI());
	this->fileContent->setFileName(this->request.getPath());
}