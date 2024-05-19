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


void WebServer::loadConfigFile(std::string configFile) // WebServer loadConfigFile
{
	this->configFilename = configFile;
	this->configFile.open(this->configFilename.c_str());
	if (!this->configFile.is_open())
	{
		std::cerr << "Error: No se ha podido abrir el archivo de configuración" << std::endl;
		exit(1);
	}
	std::stringstream buffer;
	buffer << this->configFile.rdbuf();
	this->fileContent = buffer.str();
	this->processConfigFile();
}

void WebServer::processConfigFile() // WebServer processConfigFile
{
	std::string 		line;
	std::string 		serverContentConfig;
	std::string 		aux;
	std::string serverConEPOLL_CTL_ADDtentConfig = "";
	std::istringstream	fileContentStream(fileContent);
	size_t				pos;

	while (std::getline(fileContentStream, line))
	{
		line = line.substr(0, line.find_first_of("#"));
		if (line.find("/*") != std::string::npos)
		{
			aux = line;
			while (std::getline(fileContentStream, line))
			{
				aux += line;
				if(aux.find("*/") != std::string::npos)
					break;
			}
			if(aux.find("*/") == std::string::npos)
			{
				std::cerr << "Error: Comentario no cerrado" << std::endl;
				exit(1);
			}
			aux.erase(aux.find("/*"), aux.find("*/") + 2);
			line = aux;
		}
		serverContentConfig += line;
	}
	serverContentConfig = removeBlanksAndTabs(serverContentConfig);
	while (serverContentConfig.find("server:{") != std::string::npos)
	{
		pos = serverContentConfig.find("server:{", 8);
		if(pos == std::string::npos)
			aux = serverContentConfig.substr(0, pos);
		else
			aux = serverContentConfig.substr(0, pos - 1);
		std::cout << "Server to open: " << aux << std::endl;
		servers.push_back(new Server(aux));
		if(serverContentConfig.find("server:{", 8) == std::string::npos)
			break;
		serverContentConfig = serverContentConfig.substr(serverContentConfig.find("server:{", 8), serverContentConfig.length());

	}
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
    

	/*//solo a efectos informativos
	std::cout << "Nr. of Servers launched " << this->servers.size() << std::endl;
	for (size_t i = 0; i < this->servers.size(); i++)
	{
		std::vector<int> serverFds = this->servers[i]->getServerFds();
		for (size_t j = 0; j < serverFds.size(); j++)
			std::cout << "Server " << i << " listening on " << serverFds[j] << std::endl;
			//erverSocket[serverFds[j]] = this->servers[i]->getListening(serverFds[j]);
	}
	//*/
	for (size_t i = 0; i < this->servers.size(); i++)
	{
		sk = this->servers[i]->getServerFds();
		std::cout << "Server " << i << " listening on " << sk.size() << " ports" << std::endl;
		for(size_t j = 0; j < sk.size(); j++)
		{
			ListeningSocket *tmp = this->servers[i]->getListening(sk[j]);
			serverSocket[sk[j]] = tmp;
		}
		// serverSocket.insert(serverSocket.end(), this->servers[i]->getServerFds().begin(), this->servers[i]->getServerFds().end());
	}
	std::cout << "Server launched" << std::endl;
	this->eventLoop();
}


/*
int WebServer::addConnection(int fd)
{

	std::cerr << "addConnection " << fd << std::endl;
	if (fd < 1)
		return -1;
	int i = this->getConnection(0);
	if (i == -1)
		return -1;
	else
	{
		this->client_events[i] = fd;
		return 0;
	}
}
	
int WebServer::removeConnection(int fd)
{
	if (fd < 1)
		return -1;
	int i = getConnection(fd);
	if (i == -1)
		return -1;
	this->client_events[i] = 0;
	return close(fd);
}

int WebServer::getConnection(int fd)
{
	size_t len = this->servers.size();
	for (size_t i = 0; i < len; i++)
	{
		if (this->client_events[i] == fd)
			return i;
	}
	return -1;
}
*/
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
	int num_events = 0;
	while (1)
	{
		std::cout << CHR_YELLOW << "Waiting Events" << RESET << std::endl;
		if (errno != 0)
		{
			std::cerr << " Last Error found: " << strerror(errno) << std::endl;
			errno = 0;
			std::cin.get();
		}
		num_events = waitEvent(evList);
		if (num_events == -1)
		{
			throw("Error: could not wait for events");
		}
		else
			std::cout << "Events received " << num_events << std::endl;
		for (int i = 0; i < num_events; i++)
		{
			#ifdef __APPLE__
				currfd = evList[i].ident;
				type_event = evList[i].filter;
			#elif __linux__
				currfd = evList[i].data.fd;
				type_event = evList[i].events;
			#endif
			std::cout << "Event ident " << currfd << std::endl;
			if (serverSocket.find(currfd) != serverSocket.end())
			{
				fd = acceptNewEvent(currfd);
				if (fd == -1)
					continue;
			}
			else if (type_event == (READ_EVENT))
			{
				recv(currfd, buf, sizeof(buf) * MAX_MSG_SIZE, 0);
				this->acceptedSocket[currfd]->loadRequest(buf);
				//removeEventFd(currfd,READ_EVENT);
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
					std::cout << "Data complete sent, Connection closed " << currfd << std::endl;
					removeEventFd(currfd, WRITE_EVENT);
					std::cout << "delete socket " << acceptedSocket[currfd] << std::endl;
					delete acceptedSocket[currfd];

					acceptedSocket.erase(currfd);
					//removeConnection(currfd);	
					//close(currfd);
				}
				else
				{
					std::cout << "Partial data sent, Connection renewed " << currfd << std::endl;
				//	modifEvent(evList[i], WRITE_EVENT, WRITE_EVENT);
				}
			}
			else if (type_event & END_EVENT || type_event & ERR_EVENT)
			{
				removeEventFd(currfd, READ_EVENT);
				std::cout << CHR_RED << "Connection closed " << RESET << currfd << std::endl;
				// removeConnection(currfd);
				close(currfd);
			}
		}
	}
}
