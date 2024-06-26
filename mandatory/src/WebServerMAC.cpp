/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServerMAC.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42barcelona>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 13:50:44 by eavedill          #+#    #+#             */
/*   Updated: 2024/06/30 13:50:44 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	listen(curfd, SOMAXCONN);
	while (1)
	{
		fd = accept(curfd, (struct sockaddr *)&addr, &socklen);
		if (fd < 0)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				break;
			else
			{
				std::cerr << "Error accepting connection" << std::endl;
				return fd;
			}
		}		
		fcntl(curfd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
		int nosigpipe = 1;
		setsockopt(curfd, SOL_SOCKET, SO_NOSIGPIPE, &nosigpipe, sizeof(nosigpipe));
		acceptedSocket[fd] = serverSocket[curfd]->clone(fd);
		this->addEvent(fd, EVFILT_READ);
	}
	return fd;
}
#endif
