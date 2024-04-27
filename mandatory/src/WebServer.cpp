#include "../inc/WebServer.hpp"


WebServer::WebServer(){} // WebServer constructor
WebServer::~WebServer(){} // WebServer destructor
// WebServer::WebServer(WebServer const &copy) // WebServer copy constructor
// {
// 	*this = copy;
// }
// WebServer &WebServer::operator=(WebServer const &copy) // WebServer operator=
// {
// 	if (this != &copy)
// 	{
// 		this->servers = copy.servers;
// 		this->configFile = copy.configFile;
// 		this->configFilename = copy.configFilename;
// 		this->fileContent = copy.fileContent;
// 	}
// 	return *this;
// }
void WebServer::loadConfigFile(std::string configFile) // WebServer loadConfigFile
{
	this->configFilename = configFile;
	this->configFile.open(configFile);
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
	std::string line;
	std::string aux;
	std::string serverContentConfig = "";
	std::istringstream fileContentStream(fileContent);

	while (std::getline(fileContentStream, line))
	{
		aux = removeBlanksAndTabs(line);
		if (aux.substr(0, 1) == "#" || aux.empty() ||aux.substr(0, 1) == "//" ||aux.substr(0, 1) == "}")
			continue;
		if (aux.find("server:{") != std::string::npos)
		{
			std::getline(fileContentStream, line);
			aux = removeBlanksAndTabs(line);
			while (aux.find("}") == std::string::npos)
			{
				serverContentConfig += line;
			}
			servers.push_back(new Server(serverContentConfig));
		}
	}
}

