#include "../inc/WebServer.hpp"

bool WebServer::running = true;

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

void WebServer::signalHandler(int signum)
{
	if (signum == SIGINT)
		running = false;
}

void WebServer::launchServers()
{
	std::vector<int> sk;


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

void	WebServer::eventLoop()
{
	struct kevent evList;
	signal(SIGINT, &WebServer::signalHandler);
	while (1)
	{
		std::cout << "Waiting for events" << std::endl;
		struct timespec *timeout = NULL;
		int num_events = kevent(kq, NULL, 0, &evList, MAX_EVENTS, timeout);
		if (num_events == -1)
		{
			if (errno != EINTR)
				std::cerr << "Error: could not wait for events" << std::endl;
		}
		else
			std::cout << "Events received " << num_events << std::endl;
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
		
			// if (acceptedSocket[evList.ident]->getState())
			// {
				removeFilter(evList.ident, EVFILT_WRITE);
				close(evList.ident);
			// }
		}
		if (running == false)
			break;
	}
	std::cout << "Server shutting down" << std::endl;
}

void WebServer::initalizer()
{
	std::cout << "WebServer initalized" << std::endl;
	this->mimeTypes = create_filetypes();
	this->errorPages = createHttpErrorCodes();
	this->running = true;
}
