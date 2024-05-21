#ifdef __linux__
#include "../inc/WebServer.hpp"

void WebServer::createQueue()
{
	this->kq = epoll_create(1);
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
			struct epoll_event event;
			event.events = EPOLLIN | EPOLLET; // Edge-triggered mode
			event.data.fd = serverFds[j];
			if (epoll_ctl(this->kq, EPOLL_CTL_ADD, serverFds[j], &event) == -1)
			{
				throw("Error: could not add event to kqueue");
			}
		}
	}
}

int WebServer::waitEvent(struct epoll_event *evList)
{
	int num_events = epoll_wait(this->kq, evList, MAX_EVENTS, -1);
	return num_events;
}

void WebServer::modifEvent(struct epoll_event eventList, int type)
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

void WebServer::addEvent(int fd, int type)
{
	struct epoll_event ev;
	ev.events = type;
	ev.data.fd = fd;
	if(epoll_ctl(this->kq, EPOLL_CTL_ADD, fd, &ev) == -1)
	{
		std::cerr << "Error: could not add event to queue " << std::endl;
		exit(1);
	}
	else
		std::cout << "Connection added to queue " << fd << std::endl;

}

void WebServer::removeEventFd(int fd, int type)
{
	struct epoll_event ev;
	(void) type;
	ev.data.fd = fd;
	ev.events = EPOLLONESHOT; // Edge-triggered mode
	if (epoll_ctl(this->kq, EPOLL_CTL_DEL, fd, &ev) == -1)
	{
		std::cerr << "Error: could not delete event" << std::endl;
		exit(1);
	}
}

int WebServer::acceptNewEvent(int curfd)
{
	int fd = 0;
	if(listen(serverSocket[curfd]->getFd(), SOMAXCONN) < 0)
	{
		std::cerr << "Error listening" << std::endl;
		exit(1);
	}

//	while (1)
//	{
//		if (listen(curfd, SOMAXCONN) < 0)
//		{
//			std::cerr << "Error listening" << curfd << std::endl;
//			exit(1);
//		}
		struct sockaddr_storage addr;
		socklen_t socklen = sizeof(addr);
		fd = accept(curfd, (struct sockaddr *) &addr, &socklen);
		if (fd < 0)
		{
//			if (errno == EAGAIN || errno == EWOULDBLOCK)
//				break;
//			else
//			{
				std::cerr << "Error accepting connection" << std::endl;
				return fd; // Continue to the next event
//			}
		}
		std::cout << "Connection accepted " << fd << std::endl;

		fcntl(fd, F_SETFL, O_NONBLOCK, O_CLOEXEC);
		acceptedSocket[fd] = serverSocket[curfd]->clone(fd);
		std::cout << "Connection accepted " << fd << " socket ptr " << acceptedSocket[fd] << std::endl;

		std::cout << "-- Client ptr  " << acceptedSocket[fd]->getClientPtr() << std::endl;
		this->addEvent(fd, EPOLLIN | EPOLLET);
//		if (addConnection(fd) == 0)
//		{
//			std::cout << "Connection added " << fd << std::endl;
//			//evList[i].events = EPOLLIN | EPOLLET; // Edge-triggered mode
//			this->addEvent(fd, EPOLLIN | EPOLLET);
//		}
//	}
	return fd;
}

#endif
