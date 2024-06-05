#include "../inc/WebServer.hpp"

WebServer::WebServer()
{
	std::cerr << "WebServer created" << std::endl;
}

WebServer::~WebServer()
{
	for (size_t i = 0; i < this->servers.size(); i++)
		delete this->servers[i];
    for (std::map<int, ListeningSocket *>::iterator it = serverSocket.begin(); it != serverSocket.end(); ++it)
		delete it->second;
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

bool WebServer::ExitFlag = false;

void	WebServer::eventLoop()
{
	#ifdef __APPLE__
		struct kevent evList[MAX_EVENTS];
	#elif __linux__
		struct epoll_event evList[MAX_EVENTS];
	#endif
	signal(SIGINT, &WebServer::exit_handler);
	int currfd = 0;
	int fd;
	int type_event;
	int flag;
	int num_events = 0;
	while (!WebServer::ExitFlag)
	{
		num_events = waitEvent(evList);
		if (num_events == -1)
		{
			//throw("Error: could not wait for events");
			continue ;
		}
		std::cout << "Event " << num_events << std::endl;
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
			if (serverSocket.find(currfd) != serverSocket.end())//&& acceptedSocket.find(currfd) == acceptedSocket.end())
			{
				fd = acceptNewEvent(currfd);
				if (fd == -1)
					continue;
			}
			else if (flag & END_EVENT || flag & ERR_EVENT)
			{
				delete acceptedSocket[currfd];
				acceptedSocket.erase(currfd);
				break ;
			}
			else if (type_event == (READ_EVENT))
			{
				if (this->acceptedSocket[currfd]->receive() == true)
				{	
					this->acceptedSocket[currfd]->loadRequest();
					#ifdef __APPLE__
						modifEvent(evList[i], READ_EVENT, WRITE_EVENT);
					#elif __linux__
						modifEvent(evList[i], WRITE_EVENT);
					#endif
				}
				else 
					continue;
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

void WebServer::exit_handler(int signum)
{
	if (signum == SIGINT)
	{
		std::cerr << "Exiting..." << std::endl;
		WebServer::ExitFlag = true;
	}
}
