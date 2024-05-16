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
	for (size_t i = 0; i < MAX_EVENTS; i++) {
    	client_events[i] = 0; // Initialize all elements to zero
	}

	this->kq = kqueue();
	this->addEventSet();
	std::cout << "Nr. of Servers launched " << this->servers.size() << std::endl;
	for (size_t i = 0; i < this->servers.size(); i++)
	{
		std::vector<int> serverFds = this->servers[i]->getServerFds();
		for (size_t j = 0; j < serverFds.size(); j++)
			std::cout << "Server " << i << " listening on " << serverFds[j] << std::endl;
	}
	for (size_t i = 0; i < this->servers.size(); i++)
	{
		sk = this->servers[i]->getServerFds();
		std::cout << "Server " << i << " listening on " << sk.size() << " ports" << std::endl;
		for(size_t j = 0; j < sk.size(); j++)
			serverSocket[sk[j]] = this->servers[i]->getListening(sk[j]);
	}
	std::cout << "Server launched" << std::endl;
	this->eventLoop();
}

// struct sockaddr_in WebServer::convertHost(std::string hostname, int port)
// {
// 	 struct addrinfo hints, *res;
// 	 struct sockaddr_in serverAddr;
// 	 memset(&hints, 0, sizeof(hints));
// 	 memset(&serverAddr, 0, sizeof(serverAddr));
// 	 hints.ai_family = AF_INET;
// 	 hints.ai_socktype = SOCK_STREAM;

// 	 if (getaddrinfo(hostname.c_str(), NULL, &hints, &res) != 0)
// 	 {
// 		 std::cerr << "Error: could not get address info" << std::endl;
// 		 exit(1);
// 	 }
// 	 serverAddr.sin_family = AF_INET;
// 	 serverAddr.sin_port = htons(port);
// 	 serverAddr.sin_addr = ((struct sockaddr_in *)res->ai_addr)->sin_addr;
// 	// freeaddrinfo(res);
// 	 return serverAddr;
// }

void	WebServer::addEventSet()
{
	for (size_t i = 0; i < this->servers.size() ; i++)
	{
		std::vector<int> serverFds = this->servers[i]->getServerFds();
		for (size_t j = 0; j < serverFds.size(); j++)
		{
			struct kevent evSet;
			
			EV_SET(&evSet, serverFds[j], EVFILT_READ, EV_ADD | EV_CLEAR, NOTE_WRITE, 0, NULL);
			if (kevent(this->kq, &evSet, 1, NULL, 0, NULL) == -1)
			{
				std::cerr << "Error: could not add event to kqueue" << std::endl;
				exit(1);
			}
		}
	}
}

void WebServer::addFilter(int fd, int type)
{
	struct kevent evSet;
	EV_SET(&evSet,fd, type, EV_ADD, 0, 0, NULL);
	if (kevent(this->kq, &evSet, 1, NULL, 0, NULL) == -1)
	{
		std::cerr << "Error: could not add event" << std::endl;
		exit(1);
	}
}

void WebServer::removeFilter(int fd, int type)
{
	struct kevent evSet;
	EV_SET(&evSet, fd, type, EV_DELETE, 0, 0, NULL);
	if (kevent(this->kq, &evSet, 1, NULL, 0, NULL) == -1)
	{
		std::cerr << "Error: could not delete event" << std::endl;
		exit(1);
	}
}

void	WebServer::newConnection(int fd)
{
	struct sockaddr_storage addr;
	socklen_t socklen = sizeof(addr);
	int socketFd = accept(fd, (struct sockaddr *) &addr, &socklen);
	if (socketFd < 0)
	{
		std::cerr << "Error accepting connection" << std::endl;
		return;
	}
	if (fcntl(socketFd, F_SETFL, O_NONBLOCK, FD_CLOEXEC) < 0)
	{
		std::cerr << "Error: could not set socket to non-blocking" << std::endl;
		exit(1);
	}
	acceptedSocket.insert(std::pair<int, ListeningSocket *>(socketFd, serverSocket[fd]->clone()));
	addFilter(socketFd, EVFILT_READ);
}

// int	WebServer::getServerSocket(int fd)
// {

// }

void	WebServer::eventLoop()
{
	struct kevent evList;
	while (1)
	{
		std::cout << "Waiting for events" << std::endl;
		struct timespec *timeout = NULL;
		int num_events = kevent(kq, NULL, 0, &evList, MAX_EVENTS, timeout);
		if (num_events == -1)
		{
			std::cerr << "Error: could not wait for events" << std::endl;
			exit(1);
		}
		else
		std::cout << "Events received " << num_events << std::endl;
		//if (getServerSocket(evList.ident))
		if (serverSocket.find(evList.ident) != serverSocket.end())	
			newConnection(evList.ident);
		else if (evList.filter == EVFILT_READ)
		{
	
			char buf[MAX_MSG_SIZE] = {0};
		
			recv(evList.ident, buf, sizeof(buf), 0);
			

			this->acceptedSocket[evList.ident]->loadRequest(buf);
			removeFilter(evList.ident, EVFILT_READ);
			addFilter(evList.ident, EVFILT_WRITE);
		}
		else if (evList.filter == EVFILT_TIMER || evList.flags & EV_EOF)
		{
			std::cout << "EOF received" << std::endl;
			// removeFilter(evList[i], EVFILT_READ)
			close(evList.ident);
		}
		else if (evList.filter == EVFILT_WRITE)
		{
			acceptedSocket[evList.ident]->sendData(evList.ident);
			// delete acceptedSocket[evList[i].ident];
			acceptedSocket.erase(evList.ident);
			// delete acceptedSocket[evList[i].ident]; d
			struct timespec timeout;
			timeout.tv_sec = 5;
			timeout.tv_nsec = 0;
			struct kevent ev;
			EV_SET(&ev, evList.ident, EVFILT_TIMER, EV_ADD | EV_ENABLE | EV_ONESHOT, \
				0, 5000, &timeout);
			if (kevent(this->kq, &ev, 1, NULL, 0, NULL) == -1)
				;

			removeFilter(evList.ident, EVFILT_WRITE);
			close(evList.ident);
		}
	}
}
