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
		serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); //Add getHost() to get the ip address of the host
		serverAddr.sin_port = htons(this->servers[i].getPort());

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


void WebServer::eventLoop()
{
	std::vector<int> client_sockets;

    while (1)
    {
		// for (std::vector<int>::iterator it = serverSocket.begin(); it != serverSocket.end(); it++)
		// {
		// 	std::cout << *it << std::endl;
		// }
	    struct kevent evList[MAX_EVENTS];
		int num_events = kevent(this->kq, NULL, 0, evList, MAX_EVENTS, NULL);
		if (num_events < 0)
		{
			std::cerr << "Error: could not get event" << std::endl;
			break;
		}
		std::cout << "HI" << std::endl;
		for (int i = 0; i < num_events; i++)
		{
			std::cout << evList[i].ident << std::endl;

			if (evList[i].flags & EV_ERROR)
					std::cout << "Socket was deleted\n";
			else if (evList[i].filter == EVFILT_TIMER)
					std::cout << "TIME\n" ;
			std::cout << "HI" << std::endl;
			if (std::find(serverSocket.begin(), serverSocket.end(), evList[i].ident) != serverSocket.end() && \
				std::find(client_sockets.begin(), client_sockets.end(), evList[i].ident) == client_sockets.end() )
			{
				std::cout << "HI2" << std::endl;
			
				std::cout << evList[i].ident << std::endl;
				struct sockaddr_in client_address;
				socklen_t client_address_len = sizeof(client_address);
				int client_socket = accept(evList[i].ident, (struct sockaddr *)&client_address, &client_address_len);
				if (client_socket == -1) {
					perror("Accept failed");
					continue;
				}
				// printf("New connection from: %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

				// Set client socket to non-blocking mode
				if (fcntl(client_socket, F_SETFL, O_NONBLOCK)==-1)
				{
					std::cerr << "Example " << std::endl;
				}

				// Register the new client socket for READ events
				struct kevent client_event;


				EV_SET(&client_event, client_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
				if (kevent(this->kq, &client_event, 1, NULL, 0, NULL) == -1)
				{
					std::cerr << "Error: could not add event" << std::endl;
					exit(1);
				}

				client_sockets.push_back(client_socket);
			}
			
			else if (std::find(client_sockets.begin(), client_sockets.end(), evList[i].ident) != client_sockets.end())
			{
				if (evList[i].filter == EVFILT_READ)
				{
					char buffer[MAX_MSG_SIZE];
					ssize_t bytes = recv(evList[i].ident, buffer, sizeof(buffer), 0);
						if (bytes == -1) {
						perror("Receive error");
						// close(fd);
					} else if (bytes == 0) {
						std::cerr << "hello" << std::endl;
						client_sockets.erase(std::find(client_sockets.begin(), client_sockets.end(), evList[i].ident));
						close(evList[i].ident);
					}
					std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: keep-alive\r\nKeep-Alive: timeout=5\r\nContent-Length: 50\r\n\r\n<html><body><h1>Hello, World!</h1></body></html>\r\n";
					// Echo back the received data
					if (send(evList[i].ident, response.c_str(), response.size(), 0) == -1)
					{
						perror("Send error");	
					}
					struct kevent evEvent;
					EV_SET(&evEvent, evList[i].ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
					// kevent(kq, &evEvent, 1, NULL, 0, NULL);		
					if (kevent(this->kq, &evEvent, 1, NULL, 0, NULL) == -1)
					{
						std::cerr << "Error: could not add event to kqueue" << std::endl;
						exit(1);
					}
					close(evList[i].ident);
				}
			}
		}
	}
}

// void	WebServer::eventLoop()
// {
// 	// struct keventc evSet;
// 	struct kevent evList[MAX_EVENTS];


// 	while (1)
// 	{
// 		int num_events = kevent(kq, NULL, 0, evList, MAX_EVENTS, NULL);
// 		for (int i = 0; i < num_events; i++)
// 		{
// 			std::cout << "Event " << i << std::endl;
// 			std::cout << "Event ident " << evList[i].ident << std::endl;
// 			if (std::find(this->serverSocket.begin(), this->serverSocket.end(), (int)evList[i].ident) != this->serverSocket.end())
// 			{
// 				struct sockaddr_storage addr;
// 				socklen_t socklen = sizeof(addr);
// 				char ip[INET6_ADDRSTRLEN];
// 				int fd = accept(evList[i].ident, (struct sockaddr *) &addr, &socklen);
// 				 if (fd < 0)
// 				{
// 					std::cerr << "Error accepting connection" << std::endl;
// 					continue; // Continue to the next event
// 				}
// 				inet_ntop(addr.ss_family, &((struct sockaddr_in *)&addr)->sin_addr, ip, sizeof(ip));
// 				std::cout << "Connection from " << ip << std::endl;
// 				if (addConnection(fd) == 0)
// 				{
// 					std::cout << "Connection accepted " << fd << std::endl;
// 					EV_SET(evList, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
// 					kevent(kq, evList, 1, NULL, 0, NULL);
// 				}
// 				// else {
// 				// 	printf("connection refused.\n");
// 				// 	close(fd);
// 				// }
// 			}
// 			else if (evList[i].filter == EVFILT_READ)
// 			{
// 				if (evList[i].flags & EV_EOF)
// 				{
// 					removeFilter(evList[i], EVFILT_READ);
// 					removeConnection(evList[i].ident);
// 				}
// 				else
// 				{
// 					// int accepted;
// 					int fd = evList[i].ident;
// 					char buf[MAX_MSG_SIZE] = {0};
// 					// if (accepted = accept(fd, (struct sockaddr *)&addr, &socklen) < 0)
// 					// {
// 					// 	std::cerr << "Error: could not accept connection" << std::endl;
// 					// 	exit(1);
// 					// }
// 					if (recv(fd, buf, sizeof(buf) * MAX_MSG_SIZE, 0) > 0)
// 						std::cout << buf << std::endl;
// 					// send(fd, response.c_str(), response.length(), 0);
					
// 					addFilter(evList[i], EVFILT_WRITE);
// 					removeFilter(evList[i], EVFILT_READ);
// 				}
// 				// removeConnection(fd);
// 			}
// 			else if (evList[i].filter == EVFILT_WRITE)
// 			{
// 				if (evList[i].flags & EV_EOF)
// 				{
// 					std::cout <<  "client closed connection before response" << std::endl;
// 					removeFilter(evList[i], EVFILT_WRITE);
// 					// removeConnection(evList[i].ident);
// 				}
// 				else
// 				{
// 					std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello, World!</h1></body></html>\r\n";
// 					int fd = evList[i].ident;
// 					send(fd, response.c_str(), response.length(), 0);
// 					removeFilter(evList[i], EVFILT_WRITE);
// 					removeConnection(fd);
// 				}
// 			}
// 		}
// 	}
// }

// void WebServer::eventLoop()
// {
// 	int eventSocket;
// 	int num_events;
	
// 	while (true)
// 	{
// 		struct timespec *timeout = NULL;
// 		if ((num_events = kevent(this->kq, NULL, 0, eventSet, 1, timeout)) < 0)
// 			continue;
// 		for (int i = 0; i < num_events; i++)
// 		{
// 			if ((eventSocket = getConnection((int)eventSet[i].ident)) < 1)
// 			{

// 				if (addConnection(eventSet[i].ident) == -1)
// 				{
// 					std::cerr << "Error: could not add connection" << std::endl;
// 					exit(1);
// 				}
// 				struct sockaddr_in client_address;
// 				socklen_t client_address_len = sizeof(client_address);
// 				int connfd = accept(eventSet[i].ident, (struct sockaddr *)&client_address, &client_address_len);
// 				EV_SET(eventSet, connfd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
// 				if (kevent(kq, eventSet, 1, NULL, 0, NULL) == -1)
// 					;//throw Webserver::KeventError();
// 			}
// 			else if (eventSet[i].filter == EVFILT_TIMER || eventSet[i].flags & EV_EOF)
// 			{
// 				if (fcntl(eventSet[i].ident, F_GETFD) > -1)
// 				{
// 					std::cout << "Disconnect for fd " << getConnection(eventSet[i].ident) << std::endl;
// 					close(eventSet[i].ident);
// 				}
// 			}
// 			else if (eventSet[i].filter == EVFILT_READ)
// 			{
// 				std::cout << "Read event" << getConnection(eventSet[i].ident) << std::endl;
// 				int fd = this->eventSet[i].ident;
// 				struct sockaddr_in client_address;
// 				socklen_t client_address_len = sizeof(client_address);
// 				int accepted;
// 				if ((accepted = accept(fd, (struct sockaddr *)&client_address, &client_address_len)) < 0)
// 				{
// 					std::cerr << "Error: could not accept connection" << std::endl;
// 					exit(1);
// 				}
// 				fcntl(accepted, F_SETFL, O_NONBLOCK);
				
// 				// printf("client #%d\n", getConnection(fd));
// 				// fflush(stdout);
// 				char buf[MAX_MSG_SIZE];
// 				//re(accepted, response.c_str(), respo
// 				recv(accepted, buf, sizeof(buf), 0);
// 				std::cout << buf << std::endl;
		
// 				EV_SET(eventSet, accepted, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
// 				if (kevent(this->kq, eventSet, 1, NULL, 0, NULL) == -1)
// 				{
// 					std::cerr << "Error: could not add event" << std::endl;
// 					exit(1);
// 				}
// 			}
// 			else if (eventSet[i].filter == EVFILT_WRITE)
// 			{
// 				std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello, World!</h1></body></html>\r\n";
// 				int fd = this->eventSet[i].ident;
// 				send(fd, response.c_str(), response.length(), 0);
// 				EV_SET(eventSet, fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
// 				if (kevent(this->kq, eventSet, 1, NULL, 0, NULL) == -1)
// 				{
// 					std::cerr << "Error: could not delete event" << std::endl;
// 					exit(1);
// 				}
// 				// removeConnection(fd);
// 				close(fd);
// 			}
// 		}
// 	}
// }

// void WebServer::eventLoop()
// {
// 	struct kevent eventList[MAX_EVENTS];
// 	struct kevent eventSet;
	
// 	while (true)
// 	{
// 		int num_events = kevent(this->kq, NULL, 0, eventList, MAX_EVENTS, NULL);
// 		for (int event = 0; event < num_events; event++)
// 		{
			
// 			for (int socket_fd = 0; socket_fd < (int)this->serverSocket.size(); socket_fd++)
// 			{
// 				if ((int)eventList[event].ident == this->serverSocket[socket_fd])
// 				{
// 					struct sockaddr_in client_address; // no se para que sirve
// 					socklen_t client_address_len = sizeof(client_address); // no se para que sirve
// 					int fd = accept(this->serverSocket[socket_fd], (struct sockaddr *)&client_address, &client_address_len);
// 					if (fd < 0)
// 					{
// 						std::cerr << "Error: could not accept connection" << std::endl;
// 						exit(1);
// 					}
// 					if (addConnection(fd) == 0)
// 					{
// 						int flags = fcntl(fd, F_GETFL, 0);
//                         fcntl(fd, F_SETFL, flags | O_NONBLOCK);
// 						EV_SET(&eventSet, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
// 						kevent(this->kq, &eventSet, 1, NULL, 0, NULL);
// 						std::cout << "Connection accepted" << std::endl;
// 						// serverAccepted.push_back(fd);
// 						std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello, World!</h1></body></html>\r\n";
// 						// int s = eventList[event].ident;
// 						char buf[MAX_MSG_SIZE];
// 						recv(fd, buf, sizeof(buf) - 1, 0);
// 						send(fd, response.c_str(), response.length(), 0);
// 						// close(fd);
// 						// serverAccepted.pop_back();
// 					}
// 					else
// 					{
// 						std::cerr << "Connection refused" << std::endl;
// 						close(fd);
// 					}
// 				}
// 			}
// 			for (int socket_fd = 0; socket_fd < (int)this-> serverAccepted.size(); socket_fd++)
// 			{
// 				if (eventList[event].flags & EV_EOF)
// 				{
// 					int fd = eventList[event].ident;
// 					printf("client #%d disconnected.\n", getConnection(fd));
// 					EV_SET(&eventSet, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
// 					kevent(kq, &eventSet, 1, NULL, 0, NULL);
// 					removeConnection(fd);
//        			}
//        			else if (eventList[event].filter == EVFILT_READ)
// 				{
// 					int s = eventList[event].ident;
// 					char buf[MAX_MSG_SIZE];
					
// 					// buf[bytes_read - 1] = 0;
// 					printf("client #%d: %s", getConnection(s), buf);
// 					//fflush(stdout);
// 					std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello, World!</h1></body></html>\r\n";
// 					send(s, response.c_str(), response.length(), 0);
// 				}
// 			}
// 		}
// 	}
// }
