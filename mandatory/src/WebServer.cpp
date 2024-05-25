#include "../inc/WebServer.hpp"

WebServer::WebServer()
{
	std::cerr << "WebServer created" << std::endl;
}

WebServer::~WebServer()
{
	std::cerr << "WebServer destroyed" << std::endl;
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
    
	for (size_t i = 0; i < this->servers.size(); i++)
	{
		sk = this->servers[i]->getServerFds();
		for(size_t j = 0; j < sk.size(); j++)
		{
			ListeningSocket *tmp = this->servers[i]->getListening(sk[j]);
			serverSocket[sk[j]] = tmp;
		}
	}
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
	int flag;
	int num_events = 0;
	while (1)
	{
		num_events = waitEvent(evList);
		if (num_events == -1)
		{
			throw("Error: could not wait for events");
		}
		for (int i = 0; i < num_events; i++)
		{
			#ifdef __APPLE__
				currfd = evList[i].ident;
				type_event = evList[i].filter;
				flag = evList[i].flags;
			#elif __linux__
				currfd = evList[i].data.fd;
				type_event = evList[i].events;
				flag = evList[i].events;
			#endif
			if (serverSocket.find(currfd) != serverSocket.end())
			{
				fd = acceptNewEvent(currfd);
				if (fd == -1)
					continue;
			}
			else if (flag & END_EVENT || flag & ERR_EVENT)
			{
				delete acceptedSocket[currfd];
				acceptedSocket.erase(currfd);
			}
			else if (type_event == (READ_EVENT))
			{
				recv(currfd, buf, sizeof(buf) * MAX_MSG_SIZE, 0);
				this->acceptedSocket[currfd]->loadRequest(buf);
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
					delete acceptedSocket[currfd];
					acceptedSocket.erase(currfd);
				}
			}
			else
			{
				std::cerr << "Unknown event " << type_event << std::endl;
			}
		}
	}
}
