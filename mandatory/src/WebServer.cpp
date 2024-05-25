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

void WebServer::launchServers()
{
	std::vector<int> sk ;
	for (size_t i = 0; i < MAX_CLIENTS; i++) {
    	client_events[i] = 0; // Initialize all elements to zero
	}
    try
    {
	    this->createQueue();
	    this->addEventSet();
    }
    catch(const std::exception& e)
    {
        std::cerr << CHR_RED"ERROR: " << e.what() << RESET << '\n';
        exit(1);
    }
    

	/*//solo a efectos informativos
	std::cout << "Nr. of Servers launched " << this->servers.size() << std::endl;
	for (size_t i = 0; i < this->servers.size(); i++)
	{
		std::vector<int> serverFds = this->servers[i]->getServerFds();
		for (size_t j = 0; j < serverFds.size(); j++)
			std::cout << "Server " << i << " listening on " << serverFds[j] << std::endl;
	}
	//*/
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



void	WebServer::eventLoop()
{
	#ifdef __APPLE__
		struct kevent evList[MAX_EVENTS];
	#elif __linux__
		struct epoll_event evList[MAX_EVENTS];
	#endif
	char buf[MAX_MSG_SIZE] = {0};
	int currfd = 0;
	int fd;
	int type_event;
	int num_events = 0;
	while (1)
	{
		std::cout << CHR_YELLOW << "Waiting Events" << RESET << std::endl;
		if (errno != 0)
		{
			std::cerr << " Last Error found: " << strerror(errno) << std::endl;
			errno = 0;
		}
		for (size_t i = 0; i < acceptedSocket.size(); i++)
		{
			std::cout << "accepted fd pendig to be closed" << i << " " << acceptedSocket[i] << std::endl;
		}
		num_events = waitEvent(evList);
		if (num_events == -1)
		{
			throw("Error: could not wait for events");
		}
		else
			std::cout << "Events received " << num_events << std::endl;
		for (int i = 0; i < num_events; i++)
		{
			#ifdef __APPLE__
				currfd = evList[i].ident;
				type_event = evList[i].filter;
			#elif __linux__
				currfd = evList[i].data.fd;
				type_event = evList[i].events;
			#endif
			std::cout << "Event ident " << i <<  " with fd " << currfd << std::endl;
			if (evList[i].flags &  EV_EOF)
			// else if (type_event & END_EVENT || type_event & ERR_EVENT)
			{
				//removeEventFd(currfd, READ_EVENT);
				//delete acceptedSocket[currfd];
				acceptedSocket.erase(currfd);
				std::cout << CHR_RED << "Connection closed " << RESET << currfd << std::endl;
				// removeConnection(currfd);
				//close(currfd);
			}
			else if (serverSocket.find(currfd) != serverSocket.end())
			{
				fd = acceptNewEvent(currfd);
				// std::cout << "Accepted new connection " << std::endl;
				// std::cout << "created socket " << acceptedSocket[fd] << " Client ptr " << acceptedSocket[fd]->getClientPtr() << std::endl;
				if (fd == -1)
					continue;
			}
			else if (type_event == (READ_EVENT))
			{
				recv(currfd, buf, sizeof(buf) * MAX_MSG_SIZE, 0);
				this->acceptedSocket[currfd]->loadRequest(buf);
				std::cout << "load request fd = " << currfd << std::endl;
				std::cout << "load request " << acceptedSocket[currfd] << " Client ptr " << acceptedSocket[currfd]->getClientPtr() << std::endl;
				//removeEventFd(currfd,READ_EVENT);
				#ifdef __APPLE__
					modifEvent(evList[i], READ_EVENT, WRITE_EVENT);
				#elif __linux__
					modifEvent(evList[i], WRITE_EVENT);
				#endif
			}
			else if (type_event == (WRITE_EVENT))
			{
				if (acceptedSocket[currfd]->sendData(currfd))
				{
					std::cout << "Data complete sent, delete socket " << acceptedSocket[currfd] << " Client ptr " << acceptedSocket[currfd]->getClientPtr() << std::endl;
					//removeEventFd(currfd, WRITE_EVENT);
					delete acceptedSocket[currfd];
					acceptedSocket.erase(currfd);
					//removeConnection(currfd);	
					//close(currfd);
				}
				else
				{
					std::cout << "Partial data sent, Connection renewed " << currfd << acceptedSocket[currfd] << " Client ptr " << acceptedSocket[currfd]->getClientPtr() <<  std::endl;
				//	modifEvent(evList[i], WRITE_EVENT, WRITE_EVENT);
				}
			}
			else
			{
				std::cerr << "Unknown event " << type_event << std::endl;
			}
		}
	}
}
