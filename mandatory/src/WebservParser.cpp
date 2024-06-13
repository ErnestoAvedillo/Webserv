# include "../inc/WebServer.hpp"

std::string removeComments(std::string &str)
{
	std::string 		line;
	std::string 		aux;
	std::istringstream	fileContentStream(str);
	std::string 		serverContentConfig = "";
	
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
		serverContentConfig += line + "\n";
	}
	return serverContentConfig;
}

static bool mismatchedBraces(std::string fileString)
{
	if (std::count(fileString.begin(), fileString.end(), '{') != std::count(fileString.begin(), fileString.end(), '}') )
	{
		std::cerr << "Error: unbalanced braces" << std::endl;
		return false;
	}
	return true;
}

static bool endWithSemicolon(std::string &fileString)
{
	int i = 0;
	std::string			line;

	fileString = removeComments(fileString);
	fileString = removeBlanksAndTabs(fileString);

	std::istringstream fileStream(fileString);
	while (std::getline(fileStream, line))
	{
		i++;
		// if (line.length() == 0 || (!line.empty() && (line.back() == ';' || line.back() == '}' || line.back() == '{')))
		if (line.length() == 0)
			continue;
		char lastCharIter = line[line.length() - 1];
		if (!line.empty() && (lastCharIter == ';' || lastCharIter == '}' || lastCharIter == '{'))
			continue;
		else
		{
			std::cerr << "Error: Line " << i << " missing semicolon at the end: " << line << std::endl;
			return false;
		}
	}
	return true;
}

static bool isServerBlock(std::string &fileString)
{
	if (fileString.find("server:{") == std::string::npos)
	{
		std::cerr << "Error: File configuration should contain at least one server block" << std::endl;
		return false;
	}
	return true;
}

static bool hasDoublePoints(std::string &fileString)
{
	std::string			line;
	std::istringstream fileStream(fileString);
	int i = 0;
	while (std::getline(fileStream, line))
	{

		if ((line.length() == 0 || line == "{" || line == "}" || line == "};") || (!line.empty() && (line.find(":") != std::string::npos)))
			continue;
		else
		{
			std::cerr << "Error: Line " << i << " missing : between {key:value} " << line << std::endl;
			return false;
		}
	}
	return true;
}

bool WebServer::checkSyntax()
{
	if (!mismatchedBraces(this->configFileString))
		return false;
	
	if (!endWithSemicolon(this->configFileString))
		return false;

	if (!hasDoublePoints(this->configFileString))
		return false;

	if (!isServerBlock(this->configFileString))
		return false;
	return true;
}

void WebServer::loadConfigFile(std::string filename) // WebServer loadConfigFile
{
	if (!isFilePermissions(filename, R_OK))
	{
		printLog("ERROR", "File\t\t<" + filename + ">\tnot a valid file");
		exit(1);
	}
	this->configFilename = filename;
	this->configFile.open(this->configFilename.c_str());
	if (!this->configFile.is_open())
	{
		std::cerr << "Error: File not found" << std::endl;
		exit(1);
	}
	this->configFileString = std::string((std::istreambuf_iterator<char>(configFile)),std::istreambuf_iterator<char>());
}


void WebServer::processConfigFile() // WebServer processConfigFile
{
	std::string 		aux;
	size_t				pos;

	while (this->configFileString.find("server:{") != std::string::npos)
	{
		pos = this->configFileString.find("server:{", 8);
		if(pos == std::string::npos)
			aux = this->configFileString.substr(0, pos);
		else
			aux = this->configFileString.substr(0, pos - 1);
		Server *tmp = new Server(aux);
		servers.push_back(tmp);
		if(this->configFileString.find("server:{", 8) == std::string::npos)
			break;
		this->configFileString = this->configFileString.substr(this->configFileString.find("server:{", 8), this->configFileString.length());
	}
}

bool WebServer::parseInfo()
{
	for (size_t i = 0; i < this->servers.size(); i++)
	{
		std::cout << CHR_CYAN"-------------Checking Server [" << i + 1  << "]------------" RESET<< std::endl;
		this->servers[i]->checkVariables();
		// this->servers[i]->print(); // print all Server parameters for debug
		printLog("NOTICE", "OK! Server " + toString(i + 1));
		std::cout << CHR_CYAN"--------------------------------------------" RESET<< std::endl;
	}

	std::vector<std::string> ports;
	for (size_t i = 0; i < this->servers.size(); i++)
	{
		for (size_t j = 0; j < this->servers[i]->getPorts().size(); j++)
		{
			if (std::find(ports.begin(), ports.end(), this->servers[i]->getPorts()[j]) != ports.end())
			{
				printLog("ERROR", "Port " + this->servers[i]->getPorts()[j] + " duplicated");
				return false;
			}
			ports.push_back(this->servers[i]->getPorts()[j]);
		}
	}
	return (true);	
}
