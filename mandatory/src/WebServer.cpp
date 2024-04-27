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

// void WebServer::processLineConfigFile()
// {

// }

void WebServer::processConfigFile(std::string fileContent)
{
	std::string line;
	std::stringstream ss(fileContent);

	while (std::getline(ss, line, ' '))
	{
		if (line == "server")
		{
			Server server;
			std::string serverLine;
			std::getline(ss, serverLine, '}');
			std::cout << serverLine << std::endl;
		}
	}
}

void WebServer::loadConfigFile(std::string configFile)
{
	this->configFile.open(configFile);
	if (!this->configFile.is_open())
	{
		std::cerr << "Error: could not open config file" << std::endl;
		exit(1);
	}
	std::string line;
	while (std::getline(this->configFile, line))
		fileContent += line + "\n";
	std::cout << fileContent << std::endl;
	this->processConfigFile(fileContent);
	this->configFile.close();
}

// void Init();
// void Run();
// void Stop();
