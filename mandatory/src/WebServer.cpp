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
	for (std::map<int, ListeningSocket *>::iterator it = acceptedSocket.begin(); it != acceptedSocket.end(); ++it)
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

void WebServer::createListeningSockets()
{
	for (size_t i = 0; i < this->servers.size(); i++)
		this->servers[i]->createListeningSockets();
}

void WebServer::createServerSocket()
{
	std::vector<int> fd_set ;
	for (size_t i = 0; i < this->servers.size(); i++)
	{
		fd_set = this->servers[i]->getServerFds();
		for(size_t j = 0; j < fd_set.size(); j++)
		{
			ListeningSocket *tmp = this->servers[i]->getListening(fd_set[j]);
			serverSocket[fd_set[j]] = tmp;
		}
	}
}
void WebServer::launchServers()
{
	std::cout << CHR_YELLOW "Launching servers..." RESET << std::endl << std::endl;
	std::cout << "\e[4;37m    Date & time    \t\t\t\t\t\t\tport\tfd" << RESET << std::endl;

	this->createListeningSockets();
	std::cout << std::endl;
	try
	{
		this->createQueue();
		this->addEventSet();
	}
	catch(const std::exception& e)
	{
		std::cerr << CHR_RED"Error: " << e.what() << RESET << '\n';
		exit(1);
	}
	this->createServerSocket();
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
	int num_events = 0;
	signal(SIGINT, &WebServer::exit_handler);
	while (!WebServer::ExitFlag)
	{
		num_events = waitEvent(evList);
		if (num_events == -1)
			continue ;
		for (int i = 0; i < num_events; i++)
		{
			#ifdef __APPLE__
				int currfd = evList[i].ident;
				int type_event = evList[i].filter;
				int flag = evList[i].flags;
			#elif __linux__
				int currfd = evList[i].data.fd;
				int type_event = evList[i].events;
				int flag = evList[i].events;
			#endif
			if (serverSocket.find(currfd) != serverSocket.end() && acceptedSocket.find(currfd) == acceptedSocket.end())
			{
				if (acceptNewEvent(currfd) == -1)
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
				{
					if (i == num_events - 1)
					{
						// std::cerr << "DELETE 2" << std::endl;
						// delete acceptedSocket[currfd];
						// acceptedSocket.erase(currfd);
					}
					continue;
				}
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
