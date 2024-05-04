#include "../inc/WebServer.hpp"

WebServer::WebServer()
{
	std::cout << "WebServer created" << std::endl;
}

WebServer::~WebServer()
{
	std::cout << "WebServer destroyed" << std::endl;
}

WebServer::WebServer(WebServer const &copy)
{
	*this = copy;
}

WebServer &WebServer::operator=(WebServer const &copy)
{
	if (this != &copy)
	{
		this->servers = copy.servers;
	}
	return *this;
}


void WebServer::processConfigFile(std::string fileContent)
{
	std::string line;
	std::stringstream ss(fileContent);

	while (std::getline(ss, line, ' '))
	{
		if (line == "server")
		{
			Server server;
			std::string serverLine;
			std::getline(ss, serverLine, '}');
			std::cout << serverLine << std::endl;
		}
	}
}

void WebServer::loadConfigFile(std::string configFile)
{
	this->configFile.open(configFile);
	if (!this->configFile.is_open())
	{
		std::cerr << "Error: could not open config file" << std::endl;
		exit(1);
	}
	std::string line;
	while (std::getline(this->configFile, line))
		fileContent += line + "\n";
	std::cout << fileContent << std::endl;
	this->processConfigFile(fileContent);
	this->configFile.close();
}

void WebServer::launchServers()
{
	for (size_t i = 0; i < MAX_EVENTS; i++) {
    	client_events[i] = 0; // Initialize all elements to zero
	}
	std::cout << this->servers.size() << std::endl;
	std::cout << client_events[0] << std::endl;
	std::cout << client_events[1] << std::endl;
	std::cout << client_events[2] << std::endl;

	this->kq = kqueue();
	this->createSocket();
	this->addEventSet();

	std::cout << "Server launched" << std::endl;
	this->eventLoop();
}

struct sockaddr_in WebServer::convertHost(std::string hostname, int port)
{
	 struct addrinfo hints, *res;
	 struct sockaddr_in serverAddr;
	 memset(&hints, 0, sizeof(hints));
	 memset(&serverAddr, 0, sizeof(serverAddr));
	 hints.ai_family = AF_INET;
	 hints.ai_socktype = SOCK_STREAM;

	 if (getaddrinfo(hostname.c_str(), NULL, &hints, &res) != 0)
	 {
		 std::cerr << "Error: could not get address info" << std::endl;
		 exit(1);
	 }
	 serverAddr.sin_family = AF_INET;
	 serverAddr.sin_port = htons(port);
	 serverAddr.sin_addr = ((struct sockaddr_in *)res->ai_addr)->sin_addr;
	// freeaddrinfo(res);
	 return serverAddr;
}

void WebServer::createSocket()
{
	for (size_t i = 0; i < this->servers.size(); i++)
	{
		std::cout << "Creating socket for server " << servers[i].getPort() << std::endl;
		int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (socket_fd < 0)
		{
			std::cerr << "Error: could not create socket" << std::endl;
			exit(1);
		}

		if (fcntl(socket_fd, F_SETFL, O_NONBLOCK) < 0)
		{
			std::cerr << "Error" << std::endl;
			exit(1);
		}

		int enable = 1;
		setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
		
		struct sockaddr_in serverAddr;
		memset(&serverAddr, 0, sizeof(serverAddr));
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(this->servers[i].getPort());
		// serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		
		// if (this->servers[i].getHost().find_first_of(".") == std::string::npos)
		// 	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		// else 
		// {
		struct addrinfo hints, *res;
		std::memset(&hints, 0, sizeof(hints));
		if (getaddrinfo(this->servers[i].getHost().c_str(), nullptr, &hints, &res) == 0)
		{
			if (res != nullptr)
			{
				struct sockaddr_in *addr_in = (struct sockaddr_in*)res->ai_addr;
				serverAddr.sin_addr.s_addr = addr_in->sin_addr.s_addr;
				freeaddrinfo(res);
			}
		}
		else
		{
			std::cerr << "Error: could not get address info" << std::endl;
			exit(1);
		}
		if (bind(socket_fd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
		{
			std::cerr << "Error: could not bind socket" << std::endl;
			exit(1);
		}
		if (listen(socket_fd, BACKLOG) < 0)
		{
			std::cerr << "Error: could not listen on socket" << std::endl;
			exit(1);
		}
		// fcntl(socket_fd, F_SETFL, O_NONBLOCK);
		this->serverSocket.push_back(socket_fd);
		std::cout << "Socket created " << socket_fd << std::endl;
	}
}

void	WebServer::addEventSet()
{
	for (size_t i = 0; i < this->serverSocket.size() ; i++)
	{
		struct kevent evSet;
		std::cout << "Adding event for server socketnbr " << this->serverSocket[i] << std::endl;
		EV_SET(&evSet, this->serverSocket[i], EVFILT_READ, EV_ADD | EV_CLEAR, NOTE_WRITE, 0, NULL);
		if (kevent(this->kq, &evSet, 1, NULL, 0, NULL) == -1)
		{
			std::cerr << "Error: could not add event to kqueue" << std::endl;
			exit(1);
		}
	}
}

int WebServer::addConnection(int fd)
{

	std::cerr << "addConnection " << fd << std::endl;
	if (fd < 1)
		return -1;
	int i = this->getConnection(0);
	if (i == -1)
		return -1;
	else
	{
		this->client_events[i] = fd;
		return 0;
	}

}

int WebServer::removeConnection(int fd)
{
	if (fd < 1)
		return -1;
    int i = getConnection(fd);
    if (i == -1)
		return -1;
    this->client_events[i] = 0;
    return close(fd);
}

int WebServer::getConnection(int fd)
{
	for (size_t i = 0; i < this->servers.size(); i++)
	{
		if (this->client_events[i] == fd)
			return i;
	}
	return -1;
}

void WebServer::addFilter(struct kevent eventList, int type)
{
	struct kevent evSet;
	EV_SET(&evSet, eventList.ident, type, EV_ADD, 0, 0, NULL);
	if (kevent(this->kq, &evSet, 1, NULL, 0, NULL) == -1)
	{
		std::cerr << "Error: could not add event" << std::endl;
		exit(1);
	}
}

void WebServer::removeFilter(struct kevent eventList, int type)
{
	struct kevent evSet;
	EV_SET(&evSet, eventList.ident, type, EV_DELETE, 0, 0, NULL);
	if (kevent(this->kq, &evSet, 1, NULL, 0, NULL) == -1)
	{
		std::cerr << "Error: could not delete event" << std::endl;
		exit(1);
	}
}

void	WebServer::eventLoop()
{
	struct kevent evList[MAX_EVENTS];
	
	while (1)
	{
		int num_events = kevent(kq, NULL, 0, evList, MAX_EVENTS, NULL);
		for (int i = 0; i < num_events; i++)
		{
			std::cout << "Event ident " << evList[i].ident << std::endl;
			if (std::find(this->serverSocket.begin(), this->serverSocket.end(), (int)evList[i].ident) != this->serverSocket.end())
			{
				struct sockaddr_storage addr;
				socklen_t socklen = sizeof(addr);
				char ip[INET6_ADDRSTRLEN];
				int fd = accept(evList[i].ident, (struct sockaddr *) &addr, &socklen);
				 if (fd < 0)
				{
					std::cerr << "Error accepting connection" << std::endl;
					continue; // Continue to the next event
				}
				inet_ntop(addr.ss_family, &((struct sockaddr_in *)&addr)->sin_addr, ip, sizeof(ip));
				std::cout << "Connection from " << ip << std::endl;
				if (addConnection(fd) == 0)
				{
					std::cout << "Connection accepted " << fd << std::endl;
					EV_SET(evList, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
					kevent(kq, evList, 1, NULL, 0, NULL);
				}
			}
			else if (evList[i].filter == EVFILT_READ)
			{
				if (evList[i].flags & EV_EOF)
				{
					removeFilter(evList[i], EVFILT_READ);
					removeConnection(evList[i].ident);
				}
				else
				{
					int fd = evList[i].ident;
					char buf[MAX_MSG_SIZE] = {0};
				
					if (recv(fd, buf, sizeof(buf) * MAX_MSG_SIZE, 0) > 0)
						std::cout << buf << std::endl;
					std::string tmp = buf;
					//Request request(tmp);
					addFilter(evList[i], EVFILT_WRITE);
					removeFilter(evList[i], EVFILT_READ);
				}
			}
			else if (evList[i].filter == EVFILT_WRITE)
			{
				if (evList[i].flags & EV_EOF)
				{
					std::cout <<  "client closed connection before response" << std::endl;
					removeFilter(evList[i], EVFILT_WRITE);
				}
				else
				{
					std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello, World!</h1></body></html>\r\n";
					int fd = evList[i].ident;
					send(fd, response.c_str(), response.length(), 0);
					removeFilter(evList[i], EVFILT_WRITE);
					removeConnection(fd);
				}
			}
		}
	}
}
