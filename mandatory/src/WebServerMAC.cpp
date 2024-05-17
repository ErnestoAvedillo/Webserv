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
			
			EV_SET(&evSet, serverFds[j], EVFILT_READ, EV_ADD | EV_CLEAR, NOTE_WRITE, 0, NULL);
			if (kevent(this->kq, &evSet, 1, NULL, 0, NULL) == -1)
			{
				throw("Error: could not add event to kqueue");
			}
		}
	}
}

int waitEvent(struct epoll_event *evList)
{
		int num_events = kevent(kq, NULL, 0, evList, MAX_EVENTS, NULL);
		if (num_events == -1)
		{
			throw("Error: could not wait for events");
		}
		else
			std::cout << "Events received " << num_events << std::endl;
	return num_events;
}


void WebServer::modifEvent(struct kevent event, int type)
{
	struct kevent evSet;
	EV_SET(&evSet, event.ident, type, EV_ADD, 0, 0, NULL);
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
}

void WebServer::removeEventFd(int fd)
{
	struct kevent evSet;
	EV_SET(&evSet, fd, 0, EV_DELETE, 0, 0, NULL);
	if (kevent(this->kq, &evSet, 1, NULL, 0, NULL) == -1)
	{
		std::cerr << "Error: could not delete event" << std::endl;
		exit(1);
	}
}

int WebServer::acceptNewEvent(int curfd)
{
	int fd = accept(curfd, (struct sockaddr *) &addr, &socklen);
	if (fd < 0)
	{
		throw("Error accepting connection")
	}
	else
	std::cout << "Connection accepted " << fd << std::endl;
	acceptedSocket.insert(std::pair<int, ListeningSocket *>(fd, serverSocket[evList[i].ident]->clone()));
	if (addConnection(fd) == 0)
	{
		std::cout << "Connection accepted " << fd << std::endl;
		this->addEvent(fd, EVFILT_READ)
	}	
	return fd;
}

void	WebServer::eventLoop()
{
	struct kevent evList[MAX_EVENTS];
	
	while (1)
	{
		std::cout << "Waiting for events" << std::endl;
		try
		{
			num_events = this->waitEvent();
		}
		catch
		{
			std::cerr << eWhat() << std::endl;
			ecit(1);
		}

		for (int i = 0; i < num_events; i++)
		{
			std::cout << "Event ident " << evList[i].ident << std::endl;
			if (serverSocket.find(evList[i].ident) != serverSocket.end())
			{
				fd = this->acceptNewEvent(evList[i].ident)
				acceptedSocket.insert(std::pair<int, ListeningSocket *>(fd, serverSocket[evList[i].data.fd]->clone()));
				if (addConnection(fd) == 0)
				{
					std::cout << "Connection accepted " << fd << std::endl;
					this->addEvent(fd, EPOLLIN | EPOLLET);
				}
			}
			else if (evList[i].filter == EVFILT_READ)
			{
				char buf[MAX_MSG_SIZE] = {0};
			
				recv(evList[i].ident, buf, sizeof(buf) * MAX_MSG_SIZE, 0);
				this->acceptedSocket[evList[i].ident]->loadRequest(buf);
				addEventFd(evList[i].ident, EVFILT_WRITE);
				removeConnection(evList[i].ident);
			}
			else if (evList[i].filter == EVFILT_WRITE)
			{
				acceptedSocket[evList[i].ident]->sendData(evList[i].ident);
				removeEventFd(evList[i].ident);
				removeConnection(evList[i].ident);
			}
			else if (evList[i].events & EV_EOF)
			{
				removeEventFd(evList[i].data.fd);
				std::cout << "Connection closed " << evList[i].data.fd << std::endl;
				removeConnection(evList[i].data.fd);
				close(evList[i].data.fd);
			}
		}
	}
}

#endif
