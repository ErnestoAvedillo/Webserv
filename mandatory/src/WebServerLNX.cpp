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
	if (num_events == -1)
	{
		throw("Error: could not wait for events");
	}
	else
		std::cout << "Events received " << num_events << std::endl;
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

void WebServer::removeEventFd(int fd)
{
	struct epoll_event ev;

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

	while (1)
	{
		if (listen(curfd, SOMAXCONN) < 0)
		{
			std::cerr << "Error listening" << curfd << std::endl;
			exit(1);
		}
		struct sockaddr_storage addr;
		socklen_t socklen = sizeof(addr);
		fd = accept(curfd, (struct sockaddr *) &addr, &socklen);
		if (fd < 0)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				break;
			else
			{
				std::cerr << "Error accepting connection" << std::endl;
				return -1; // Continue to the next event
			}
		}
		std::cout << "Connection accepted " << fd << std::endl;

		fcntl(fd, F_SETFL, O_NONBLOCK, O_CLOEXEC);
		acceptedSocket[fd] = serverSocket[curfd]->clone();
		std::cout << "Connection accepted " << serverSocket[curfd]->getServerName() << std::endl;
		if (addConnection(fd) == 0)
		{
			std::cout << "Connection added " << fd << std::endl;
			//evList[i].events = EPOLLIN | EPOLLET; // Edge-triggered mode
			this->addEvent(fd, EPOLLIN | EPOLLET);
		}
	}
	return fd;
}

void	WebServer::eventLoop()
{
	struct epoll_event evList[MAX_EVENTS];
	char buf[MAX_MSG_SIZE] = {0};
	int fd;
	int num_events = 0;
	while (1)
	{
 		try
		{
			num_events = this->waitEvent(evList);
		}
		catch (std::exception &e)
		{
			std::cerr << e.what() << std::endl;
			exit(1);
		}
		for (int i = 0; i < num_events; i++)
			if (serverSocket.find(evList[i].data.fd) != serverSocket.end())
			{
				fd = acceptNewEvent(evList[i].data.fd);
				if(fd < 0)
				{
					std::cout << "Error in accepted socket" << std::endl;
					exit(1);
				}
				acceptedSocket.insert(std::pair<int, ListeningSocket *>(fd, serverSocket[evList[i].data.fd]->clone()));
				if (addConnection(fd) == 0)
				{
					std::cout << "Connection accepted " << fd << std::endl;
					this->addEvent(fd, EPOLLIN | EPOLLET);
				}
			}
			else if (evList[i].events == (EPOLLIN))
			{

				recv(evList[i].data.fd, buf, sizeof(buf) * MAX_MSG_SIZE, 0);
				this->acceptedSocket[evList[i].data.fd]->loadRequest(buf);
				modifEvent(evList[i], EPOLLOUT);
			}
			else if (evList[i].events == (EPOLLOUT))
			{
				acceptedSocket[evList[i].data.fd]->sendData(evList[i].data.fd);
				removeConnection(evList[i].data.fd);
				close(evList[i].data.fd);
			}
			else if (evList[i].events & EPOLLHUP || evList[i].events & EPOLLERR)
			{
				removeEventFd(evList[i].data.fd);
				std::cout << "Connection closed " << evList[i].data.fd << std::endl;
				removeConnection(evList[i].data.fd);
				close(evList[i].data.fd);
			}
	}
}

#endif
