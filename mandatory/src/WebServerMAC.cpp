#ifdef __APPLE__
#include "../inc/WebServer.hpp"

void WebServer::createQueue()
{
	this->kq = kqueue();
	if (this->kq == -1)
	{
		throw ("Error: could not create epoll");
	}
}

void	WebServer::addEventSet()
{
	for (size_t i = 0; i < this->servers.size() ; i++)
	{
		std::vector<int> serverFds = this->servers[i]->getServerFds();
		for (size_t j = 0; j < serverFds.size(); j++)
		{
			struct kevent evSet;
			
			// EV_SET(&evSet, serverFds[j], EVFILT_READ, EV_ADD | EV_CLEAR, NOTE_WRITE, 0, NULL);
			EV_SET(&evSet, serverFds[j], EVFILT_READ, EV_ADD | EV_CLEAR, 0, 0, NULL);
			if (kevent(this->kq, &evSet, 1, NULL, 0, NULL) == -1)
			{
				throw("Error: could not add event to kqueue");
			}
		}
	}
}

int WebServer::waitEvent(struct kevent *evList)
{
	int	num_events = kevent(this->kq, NULL, 0, evList, MAX_EVENTS, NULL);
	return num_events;
}


void WebServer::modifEvent(struct kevent event, int typeRem, int typeAdd)
{
	struct kevent evSet;
	(void) typeRem;
	EV_SET(&evSet, event.ident, typeAdd, EV_ADD, 0, 0, NULL);
	//removeEventFd(event.ident, typeRem);
	if (kevent(this->kq, &evSet, 1, NULL, 0, NULL) == -1)
	{
		std::cerr << "Error: could not add event" << std::endl;
		exit(1);
	}
}

void WebServer::addEvent(int fd, int type)
{
	struct kevent evSet;
	EV_SET(&evSet, fd, type, EV_ADD, 0, 0, NULL);
	if (kevent(this->kq, &evSet, 1, NULL, 0, NULL) == -1)
	{
		std::cerr << "Error: could not add event" << std::endl;
		exit(1);
	}
	else
		std::cout << CHR_GREEN << "Event added " << RESET << fd << std::endl;	
}

void WebServer::removeEventFd(int fd, int type)
{
	struct kevent evSet;
	EV_SET(&evSet, fd, type, EV_DELETE, 0, 0, NULL);
	(void) type;
	if (kevent(this->kq, &evSet, 1, NULL, 0, NULL) == -1)
	{
		std::cerr << "Error: could not delete event" << fd << std::endl;
		exit(1);
	}
}

int WebServer::acceptNewEvent(int curfd)
{
	struct sockaddr addr;
	socklen_t socklen = sizeof(addr);
	int fd = 0;
	fd = accept(curfd, (struct sockaddr *)&addr, &socklen);
	if (fd < 0)
		return fd;
	else
		std::cout << "Connection accepted " << fd << std::endl;
	acceptedSocket.insert(std::pair<int, ListeningSocket *>(fd, serverSocket[curfd]->clone(fd)));
	std::cout << "New socket created from server socket" << serverSocket[curfd] << std::endl;
	std::cout << "New socket created in acce[ted socket" << acceptedSocket[fd] << std::endl;
	this->addEvent(fd, EVFILT_READ);
	// if (addConnection(fd) == 0)
	// {
	// 	std::cout << "New socket created " << fd << std::endl;
	// 	this->addEvent(fd, EVFILT_READ);
	// }	
	return fd;
}

#endif
