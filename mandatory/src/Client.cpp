#include "../inc/Client.hpp"

Client::Client() {}

Client::Client(Server *srv)
{
	this->fileContent = new FileContent(srv);
	this->server = srv;
}

Client::Client(Receive *receive, Server *srv)

{
	this->fileContent = new FileContent(srv);
	this->server = srv;
	this->loadCompleteClient(receive);
}


Client::~Client()
{
	delete this->fileContent;
}

// Cambiar nombres a setters ?
void Client::addKeyReq(std::string const &key, std::string const &value){ this->Request[key] = value; }
void Client::addKeyType(std::string const &value) { this->Request[REQ_TYPE] = value; }
void Client::addKeyFile(std::string const &value) { this->Request[REQ_FILE] = value; }
void Client::addKeyVers(std::string const &value) { this->Request[REQ_VER] = value; }


void Client::loadCompleteClient(Receive *receiver)
{
	std::string str = receiver->getRequest();
	if (str.length() == 0)
		return ;
	std::vector<std::string> lines = splitString(str, '\n');
	std::vector<std::string> parts = splitString(lines[0], ' ');
	if (parts.size() == 3)
	{
		this->addKeyType(parts[0]);
		this->addKeyFile(parts[1]);
		this->addKeyVers(parts[2]);
	}
	for (size_t i = 1; i < lines.size(); i++)
		this->addKeyReq(lines[i].substr(0, lines[i].find(":")), lines[i].substr(lines[i].find(":") + 1, lines.size()));
	this->loadDataHeader(receiver);
}
std::string Client::normalizePath(std::string path)
{
	while (path.find("..") != std::string::npos)
		path.erase(path.find(".."), 2);

	std::vector<std::string> parts = splitString(path, '/');
	std::string normalizePath;
	for (size_t i = 0; i < parts.size(); i++)
	{
		if (i == 0)
		{
			if (parts[i] == ".")
				normalizePath = parts[i] + "/";
			else if (i == (parts.size() - 1))
				normalizePath = "/" + parts[i];
			else
				normalizePath = "/" + parts[i] + "/";
		}
		else if (i == (parts.size() - 1))
			normalizePath += parts[i];
		else
			normalizePath += parts[i] + "/";
	}
	return (normalizePath);
}

std::string Client::getFilePath()
{
	std::string filePath = normalizePath(server->getRoot()) + this->Request[REQ_FILE];
	if (filePath.at(filePath.size() - 1) == '/')
		filePath += server->getIndex();
	filePath = filePath.substr(0, filePath.find("?"));
	return (filePath);
}

std::string Client::getFileContent()
{
	std::string content;
	content = this->fileContent->getContent();
	return (content);
}

std::string Client::getAnswerToSend()
{
	std::string answer;
	std::string filePath = this->fileContent->getFileName();
	std::string file_content = getFileContent();
	if (this->fileContent->getFirstFragment())
	{
		answer = header.generateHeader() + file_content;
		this->fileContent->setFirstFragment(false);
	}
	else
		answer = file_content;

	return (answer);
}

bool Client::isSendComplete()
{
	return this->fileContent->isSendComplete();
}


int Client::isAllowedMethod(Location *location)
{
	if (this->Request[REQ_TYPE] == "GET")
	{
		if (location->getGetAllowed() == false)
		{
			header.setStatus("405 Method Not Allowed");
			return NOT_ALLOWED;
		}
	}
	else if (this->Request[REQ_TYPE] == "POST")
	{
		if (location->getPostAllowed() == false)
		{
			header.setStatus("405 Method Not Allowed");
			return NOT_ALLOWED;
		}

	}
	else if (this->Request[REQ_TYPE] == "DELETE")
	{
		if (location->getDeleteAllowed() == false)
		{
			header.setStatus("405 Method Not Allowed");
			return NOT_ALLOWED;
		}
	}
	return OK;
}

int Client::matchingLocation()
{
	std::vector<Location *> locations = this->server->getLocations();
	for (size_t i = 0; i < locations.size(); i++)
	{
		if (this->Request[REQ_FILE].find(locations[i]->getName()) != std::string::npos)
		{
			if (isAllowedMethod(locations[i]) == NOT_ALLOWED)
				return NOT_ALLOWED;
			switch (locations[i]->getLocationType())
			{
				case RETURN:
					header.setStatus("301 Moved Permanently");
					header.setAttribute("Location", locations[i]->getReturn());
					return REDIRECT;
				case ALIAS:
					replaceString(this->Request[REQ_FILE],  locations[i]->getName(), locations[i]->getAlias());
					if (!locations[i]->getIndex().empty() && isDirPermissions(this->Request[REQ_FILE], F_OK | R_OK) == true && this->Request[REQ_TYPE] != "POST")
						this->Request[REQ_FILE] += "/" + locations[i]->getIndex();
					break ;
				case ROOT:
					replaceString(this->Request[REQ_FILE],  locations[i]->getName(), locations[i]->getRoot() + locations[i]->getName());
					if (!locations[i]->getIndex().empty() && isDirPermissions(this->Request[REQ_FILE], F_OK | R_OK) == true && this->Request[REQ_TYPE] != "POST")
						this->Request[REQ_FILE] += "/" + locations[i]->getIndex();
					break ;

				default:
					std::cerr << "This is incorrect. Should not arrive here" << std::endl;
					break ;
			}
			return true;
		} 
	}
	return NO_LOCATION;

}

void Client::loadDataHeader(Receive *receiver)
{

	switch (this->matchingLocation())
	{
		case NO_LOCATION:
			this->Request[REQ_FILE] = this->server->getRoot() + this->Request[REQ_FILE];
			if (isDirPermissions(this->Request[REQ_FILE], F_OK | R_OK) == true)
				this->Request[REQ_FILE] += this->server->getIndex();
			break ;
		case NOT_ALLOWED :
			return ;
		case REDIRECT:
			return ;
	}

	this->Request[REQ_FILE] = decodeURL(this->Request[REQ_FILE]);
	// std::cout << this->Request[REQ_FILE] << std::endl;
	if (this->Request[REQ_TYPE] == "GET")
	{
		if (this->fileContent->setFileName(this->Request[REQ_FILE]))
		{
			header.setContentType(this->Request[REQ_FILE]);
			header.setLastModified(this->fileContent->getLastModified());
			header.setContentLength(this->fileContent->getContentSize());
			header.setStatus("200 OK");
			header.setServer(server->getServerName());
		}
		else
			header.setStatus("404 Not Found");
	}
	else if (this->Request[REQ_TYPE] == "POST")
	{
		if (receiver->getRequest().find("POST") != std::string::npos && receiver->getisform() == false)
		{
			std::string body = receiver->getBody().substr(receiver->getBody().find("\r\n\r\n") + 4);
			std::string postheader = receiver->getBody().substr(0, receiver->getBody().find("\r\n\r\n") + 4);

			std::vector<std::string> lines = splitString(postheader, '\n');
			for (size_t i = 0; i < lines.size(); i++)
			{
				if (lines[i].find("filename=") != std::string::npos)
				{
					std::string filename = lines[i].substr(lines[i].find("filename=") + 10, lines[i].size());
					filename = filename.substr(0, filename.find("\""));
					this->Request[REQ_FILE] += "/" + filename;
					if (access(this->Request[REQ_FILE].c_str(), F_OK) == 0)
					{
						header.setStatus("403 Forbidden");
						return;
					}
					std::fstream file(this->Request[REQ_FILE].c_str(), std::ios::out | std::ios::binary | std::ios::app);
					file.write(body.c_str(), body.size());
					file.close();
					header.setStatus("201 Created");
					return;
				}
			}
		}
		else if (receiver->getisform())
		{
			header.setStatus("201 Created");
		}
		else
			std::cout << "form: " << receiver->getBody() << std::endl;
		header.setServer(server->getServerName());
	}
	else if (this->Request[REQ_TYPE] == "DELETE")
	{
		if (std::remove(this->Request[REQ_FILE].c_str()) == 0)
			header.setStatus("200 OK");
		else
			header.setStatus("404 Not Found");
		header.setServer(server->getServerName());
	}
	else
	{
		header.setStatus("405 Method Not Allowed");
		header.setServer(server->getServerName());
	}
}
