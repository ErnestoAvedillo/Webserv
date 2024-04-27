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

// void WebServer::processLineConfigFile()
// {

// }

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
	struct kevent eventSet;

	this->kq = kqueue();
	this->createSocket();
	eventSet = addEventSet();
	if (kevent(this->kq, &eventSet, 1, NULL, 0, NULL) < 0)
	{
		std::cerr << "Error: could not add event to kqueue" << std::endl;
		exit(1);
	}
	this->eventLoop();
}

void WebServer::createSocket()
{
	for (int i = 0; i < (int)this->servers.size(); i++)
	{
		int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (socket_fd < 0)
		{
			std::cerr << "Error: could not create socket" << std::endl;
			exit(1);
		}
		int enable = 1;
		setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
		
		struct sockaddr_in serverAddr;
		memset(&serverAddr, 0, sizeof(serverAddr));
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); //Add getHost() to get the ip address of the host
		serverAddr.sin_port = htons(this->servers[i].getPort());

		int flags = fcntl(socket_fd, F_GETFL, 0);
        fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK);

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
		this->serverSocket.push_back(socket_fd);
	}
}

struct kevent	WebServer::addEventSet()
{
	struct kevent eventSet;
	
	for (int i = 0; i < (int)this->serverSocket.size() ; i++)
	{
		EV_SET(&eventSet,this->serverSocket[i], EVFILT_READ, EV_ADD, 0, 0, NULL);
	}
	return eventSet;
}

int WebServer::addConnection(int fd)
{
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
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		if (this->client_events[i] == fd)
			return i;
	}
	return -1;
}


void WebServer::eventLoop()
{
	struct kevent eventList[MAX_EVENTS];
	struct kevent eventSet;
	
	while (true)
	{
		int num_events = kevent(this->kq, NULL, 0, eventList, MAX_EVENTS, NULL);
		for (int event = 0; event < num_events; event++)
		{
			
			for (int socket_fd = 0; socket_fd < (int)this->serverSocket.size(); socket_fd++)
			{
				if ((int)eventList[event].ident == this->serverSocket[socket_fd])
				{
					struct sockaddr_in client_address; // no se para que sirve
					socklen_t client_address_len = sizeof(client_address); // no se para que sirve
					int fd = accept(this->serverSocket[socket_fd], (struct sockaddr *)&client_address, &client_address_len);
					if (fd < 0)
					{
						std::cerr << "Error: could not accept connection" << std::endl;
						exit(1);
					}
					if (addConnection(fd) == 0)
					{
						int flags = fcntl(fd, F_GETFL, 0);
                        fcntl(fd, F_SETFL, flags | O_NONBLOCK);
						EV_SET(&eventSet, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
						kevent(this->kq, &eventSet, 1, NULL, 0, NULL);
						std::cout << "Connection accepted" << std::endl;
						// serverAccepted.push_back(fd);
						std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello, World!</h1></body></html>\r\n";
						// int s = eventList[event].ident;
						char buf[MAX_MSG_SIZE];
						recv(fd, buf, sizeof(buf) - 1, 0);
						send(fd, response.c_str(), response.length(), 0);
						// close(fd);
						// serverAccepted.pop_back();
					}
					else
					{
						std::cerr << "Connection refused" << std::endl;
						close(fd);
					}
				}
			}
			for (int socket_fd = 0; socket_fd < (int)this-> serverAccepted.size(); socket_fd++)
			{
				if (eventList[event].flags & EV_EOF)
				{
					int fd = eventList[event].ident;
					printf("client #%d disconnected.\n", getConnection(fd));
					EV_SET(&eventSet, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
					kevent(kq, &eventSet, 1, NULL, 0, NULL);
					removeConnection(fd);
       			}
       			else if (eventList[event].filter == EVFILT_READ)
				{
					int s = eventList[event].ident;
					char buf[MAX_MSG_SIZE];
					
					// buf[bytes_read - 1] = 0;
					printf("client #%d: %s", getConnection(s), buf);
					//fflush(stdout);
					std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello, World!</h1></body></html>\r\n";
					send(s, response.c_str(), response.length(), 0);
				}
			}
		}
	}
}
