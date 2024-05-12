#ifdef __linux__
#include "../inc/WebServerLNX.hpp"

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


void WebServer::loadConfigFile(std::string configFile) // WebServer loadConfigFile
{
	this->configFilename = configFile;
	this->configFile.open(this->configFilename.c_str());
	if (!this->configFile.is_open())
	{
		std::cerr << "Error: No se ha podido abrir el archivo de configuración" << std::endl;
		exit(1);
	}
	std::stringstream buffer;
	buffer << this->configFile.rdbuf();
	this->fileContent = buffer.str();
	this->processConfigFile();
}

void WebServer::processConfigFile() // WebServer processConfigFile
{
	std::string 		line;
	std::string 		serverContentConfig;
	std::string 		aux;
	std::string serverConEPOLL_CTL_ADDtentConfig = "";
	std::istringstream	fileContentStream(fileContent);
	size_t				pos;

	while (std::getline(fileContentStream, line))
	{
		line = line.substr(0, line.find_first_of("#"));
		if (line.find("/*") != std::string::npos)
		{
			aux = line;
			while (std::getline(fileContentStream, line))
			{
				aux += line;
				if(aux.find("*/") != std::string::npos)
					break;
			}
			if(aux.find("*/") == std::string::npos)
			{
				std::cerr << "Error: Comentario no cerrado" << std::endl;
				exit(1);
			}
			aux.erase(aux.find("/*"), aux.find("*/") + 2);
			line = aux;
		}
		serverContentConfig += line;
	}
	serverContentConfig = removeBlanksAndTabs(serverContentConfig);
	while (serverContentConfig.find("server:{") != std::string::npos)
	{
		pos = serverContentConfig.find("server:{", 8);
		if(pos == std::string::npos)
			aux = serverContentConfig.substr(0, pos);
		else
			aux = serverContentConfig.substr(0, pos - 1);
		std::cout << "Server to open: " << aux << std::endl;
		servers.push_back(new Server(aux));
		if(serverContentConfig.find("server:{", 8) == std::string::npos)
			break;
		serverContentConfig = serverContentConfig.substr(serverContentConfig.find("server:{", 8), serverContentConfig.length());

	}
}

void WebServer::launchServers()
{
	std::vector<int> sk ;
	for (size_t i = 0; i < MAX_CLIENTS; i++) {
    	client_events[i] = 0; // Initialize all elements to zero
	}

	this->kq = epoll_create(1);
	if (this->kq == -1)
	{
		std::cerr << "Error: could not create epoll" << std::endl;
		exit(1);
	}
	this->addEventSet();
	std::cout << "Nr. of Servers launched " << this->servers.size() << std::endl;
	//solo a efectos informativos
	for (size_t i = 0; i < this->servers.size(); i++)
	{
		std::vector<int> serverFds = this->servers[i]->getServerFds();
		for (size_t j = 0; j < serverFds.size(); j++)
			std::cout << "Server " << i << " listening on " << serverFds[j] << std::endl;
			//erverSocket[serverFds[j]] = this->servers[i]->getListening(serverFds[j]);
	}
	//
	for (size_t i = 0; i < this->servers.size(); i++)
	{
		sk = this->servers[i]->getServerFds();
		std::cout << "Server " << i << " listening on " << sk.size() << " ports" << std::endl;
		for(size_t j = 0; j < sk.size(); j++)
		{
			ListeningSocket *tmp = this->servers[i]->getListening(sk[j]);
			serverSocket[sk[j]] = tmp;
		}
		// serverSocket.insert(serverSocket.end(), this->servers[i]->getServerFds().begin(), this->servers[i]->getServerFds().end());
	}
	std::cout << "Server launched" << std::endl;
	this->eventLoop();
}

void	WebServer::addEventSet()
{
	for (size_t i = 0; i < this->servers.size() ; i++)
	{
		std::vector<int> serverFds = this->servers[i]->getServerFds();
		for (size_t j = 0; j < serverFds.size(); j++)
		{
			struct epoll_event event;
			event.events = EPOLLIN | EPOLLET; // Edge-triggered mode
			event.data.fd = serverFds[j];
			if (epoll_ctl(this->kq, EPOLL_CTL_ADD, serverFds[j], &event) == -1)
			{
				std::cerr << "Error: could not add event to kqueue" << std::endl;
				exit(1);
			}
		}
	}
}

void WebServer::modifFilter(struct epoll_event eventList, int type)
{
	// struct epoll_event evSet;

	std::cout << "Connection filter add " << eventList.data.fd << std::endl;
	eventList.events = type; // Edge-triggered mode
	if (epoll_ctl(this->kq, EPOLL_CTL_MOD, eventList.data.fd, &eventList) == -1)
	{
		std::cerr << "Error: could not modify event" << std::endl;
		exit(1);
	}
}

void WebServer::addFilter(struct epoll_event eventList, int type)
{
	//struct epoll_event evSet;
	
	std::cout << "Connection filter add " << eventList.data.fd << std::endl;
	eventList.events = type; // Edge-triggered mode
	if (epoll_ctl(this->kq, EPOLL_CTL_ADD, eventList.data.fd, &eventList) == -1)
	{
		std::cerr << "Error: could not add event" << std::endl;
		exit(1);
	}
}

void WebServer::removeFilter(struct epoll_event eventList)
{
	//struct epoll_event evSet;

	//	EV_SET(&evSet, eventList.ident, type, EV_DELETE, 0, 0, NULL);
	//if (kevent(this->kq, &evSet, 1, NULL, 0, NULL) == -1)
	std::cout << "Connection removed " << eventList.data.fd << std::endl;
	eventList.events = EPOLLONESHOT; // Edge-triggered mode
	if (epoll_ctl(this->kq, EPOLL_CTL_DEL, eventList.data.fd, &eventList) == -1)
	{
		std::cerr << "Error: could not delete event" << std::endl;
		exit(1);
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
	size_t len = this->servers.size();
	for (size_t i = 0; i < len; i++)
	{
		if (this->client_events[i] == fd)
			return i;
	}
	return -1;
}

void	WebServer::eventLoop()
{
	struct epoll_event evList[MAX_EVENTS];
	char buf[MAX_MSG_SIZE] = {0};
	int fd;
	while (1)
	{
		//std::cout << "Waiting for events" << std::endl;
		int num_events = epoll_wait(this->kq, evList, MAX_EVENTS, -1);
		if (num_events == -1)
		{
			std::cerr << "Error: could not wait for events" << std::endl;
			exit(1);
		}
		else
			std::cout << "Events received " << num_events << std::endl;
		for (int i = 0; i < num_events; i++)
		{
			std::cout << "Events detected " << evList[i].data.fd << std::endl;
		}
		for (int i = 0; i < num_events; i++)
		{
			std::cout << "Event ident " << evList[i].data.fd << std::endl;
			printf(" fd=%d; events: %s%s%s%s\n", evList[i].data.fd,
				   (evList[i].events & EPOLLIN) ? "EPOLLIN ": "",
				   (evList[i].events & EPOLLOUT) ? "EPOLLOUT " : "",
				   (evList[i].events & EPOLLHUP) ? "EPOLLHUP " : "",
				   (evList[i].events & EPOLLERR) ? "EPOLLERR " : "");
			if (serverSocket.find(evList[i].data.fd) != serverSocket.end())
			{
				struct sockaddr_storage addr;
				socklen_t socklen = sizeof(addr);
				// char ip[INET6_ADDRSTRLEN];
				if(listen(serverSocket[evList[i].data.fd]->getFd(), SOMAXCONN) < 0)
				{
					std::cerr << "Error listening" << std::endl;
					exit(1);
				}
				while (1)
				{
					fd = accept(evList[i].data.fd, (struct sockaddr *) &addr, &socklen);
					if (fd < 0)
					{
						if (errno == EAGAIN || errno == EWOULDBLOCK)
							break;
						else
						{
							std::cerr << "Error accepting connection" << std::endl;
							exit (1); // Continue to the next event
						}
					}
					std::cout << "Connection accepted " << fd << std::endl;
					fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
					acceptedSocket.insert(std::pair<int, ListeningSocket *>(fd, serverSocket[evList[i].data.fd]->clone()));
					if (addConnection(fd) == 0)
					{
						std::cout << "Connection added " << fd << std::endl;
						evList[i].events = EPOLLIN | EPOLLET; // Edge-triggered mode
						epoll_ctl(this->kq, EPOLL_CTL_ADD, fd, evList);
					}
				}
			}
			else if (evList[i].events == (EPOLLIN | EPOLLET))
			{
				if (evList[i].events & EPOLLHUP || evList[i].events & EPOLLERR)
				{
					removeFilter(evList[i]);
					removeConnection(evList[i].data.fd);
				}
				else
				{
					recv(evList[i].data.fd, buf, sizeof(buf) * MAX_MSG_SIZE, 0);
					// if (recv(evList[i].ident, buf, sizeof(buf) * MAX_MSG_SIZE, 0) > 0)
						// std::cout << buf << std::endl;
					
					this->acceptedSocket[evList[i].data.fd]->loadRequest(buf);
					// std::string tmp = buf;
					//Request request(tmp);
					modifFilter(evList[i], EPOLLOUT | EPOLLET);
					//removeFilter(evList[i]);
				}
			}
			else if (evList[i].events == (EPOLLOUT | EPOLLET))
			{
				if (evList[i].events & EPOLLHUP || evList[i].events & EPOLLERR)
				{
					std::cout <<  "client closed connection before response" << std::endl;
					removeFilter(evList[i]);
				}
				else
				{
					
					acceptedSocket[evList[i].data.fd]->sendData(evList[i].data.fd);
					// std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello, World!</h1></body></html>\r\n";
					// send(evList[i].ident, response.c_str(), response.length(), 0);
					// std::cout << "Response sent " << this->serverSocket[fd]->buffer <<  std::endl;
					removeFilter(evList[i]);
					removeConnection(evList[i].data.fd);
				}
			}
		}
		for (int i = 0; i < num_events; i++)
		{
			epoll_ctl(this->kq, EPOLL_CTL_DEL, evList[i].data.fd, evList);
			std::cout << "Events deleted " << evList[i].data.fd << std::endl;
		}
	}
}
#endif
