#include "../inc/Client.hpp"

Client::Client() {}

Client::Client(Server *srv)
{
	this->fileContent = new FileContent(srv);
	this->server = srv;
}

Client::~Client()
{
	delete this->fileContent;
}

// Cambiar nombres a setters ?
// void Client::addKeyReq(std::string const &key, std::string const &value){ this->Request[key] = value; }
// void Client::addKeyType(std::string const &value) { this->Request[REQ_TYPE] = value; }
// void Client::addKeyFile(std::string const &value) { this->Request[REQ_FILE] = value; }
// void Client::addKeyVers(std::string const &value) { this->Request[REQ_VER] = value; }

void Client::setServer(Server *srv) { this->server = srv; }

static std::string getMimeType(std::string contentType)
{
	size_t point = contentType.find_last_of(".");
	std::string extension = contentType.substr(point + 1, contentType.size());

	/* Create once only */
	std::map<std::string, std::string> Mimetype = create_filetypes();
	if (Mimetype.find(extension) != Mimetype.end())
		return(Mimetype[extension]);
	else
		 return ("text/html");
}

void Client::matchServerName(std::vector<Server *> servers)
{
	std::string server_name = request.getAttribute("Host").substr(0, request.getAttribute("Host").find(":"));
	if (server_name.empty())
		return ;
	for (size_t i = 0; i < servers.size(); i++)
	{
		if (servers[i]->getServerName() == server_name)
		{
			this->setServer(servers[i]);
			break;
		}
	}
}

void Client::loadCompleteClient(Receive *receiver, std::vector<Server *> servers)
{
	this->setRequestHeader(receiver->getRequest());
	matchServerName(servers);
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
	std::string filePath = normalizePath(server->getRoot()) + this->request.getPath();
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
		if (response.getContentType().find("video/") != std::string::npos && this->request.getAttribute("Sec-Fetch-Dest").find("document") != std::string::npos)
			response.setAttribute("Accept-Ranges", "bytes");
		answer = response.generateHeader() + file_content;
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


void Client::setRequestHeader(std::string requestHeader)
{
	this->request = Header(requestHeader);
}

int Client::isAllowedMethod(Location *location)
{
	if (this->request.getMethod() == "GET")
	{
		if (location->getGetAllowed() == false)
		{
			response.setStatus("405 Method Not Allowed");
			return NOT_ALLOWED;
		}
	}
	else if (this->request.getMethod() == "POST")
	{
		if (location->getPostAllowed() == false)
		{
			response.setStatus("405 Method Not Allowed");
			return NOT_ALLOWED;
		}

	}
	else if (this->request.getMethod() == "DELETE")
	{
		if (location->getDeleteAllowed() == false)
		{
			response.setStatus("405 Method Not Allowed");
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
		if (this->request.getPath().find(locations[i]->getName()) != std::string::npos)
		{
			if (isAllowedMethod(locations[i]) == NOT_ALLOWED)
				return NOT_ALLOWED;
			this->fileContent->setAutoIndex(locations[i]->getAutoIndex());
			this->fileContent->setIsCGI(locations[i]->getIsCgi());
			ExtendedString tmp;
			switch (locations[i]->getLocationType())
			{
				case RETURN:
					response.setStatus("301 Moved Permanently");
					response.setAttribute("Location", locations[i]->getReturn());
					return REDIRECT;
				case ALIAS:
					tmp = this->request.getPath();
					tmp.replaceString(locations[i]->getName(), locations[i]->getAlias() + "/");
					if (!locations[i]->getIndex().empty() && isDirPermissions(tmp, F_OK | R_OK) == true && this->request.getMethod() != "POST")
						tmp += "/" + locations[i]->getIndex();
					request.setPath(tmp);
					// findCgiExtension(this->request.getPath(), this->locations[i]->getCgiExtension());
					break ;
				case ROOT:
					tmp = this->request.getPath();
					tmp.replaceString(locations[i]->getName(), locations[i]->getRoot()  + locations[i]->getName());
					if (!locations[i]->getIndex().empty() && isDirPermissions(tmp, F_OK | R_OK) == true && this->request.getMethod() != "POST")
						tmp += "/" + locations[i]->getIndex();
					request.setPath(tmp);
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

off_t getFileSize(const std::string &filename)
{
	struct stat stat_buf;
	int rc = stat(filename.c_str(), &stat_buf);
	return rc == 0 ? stat_buf.st_size : -1;
}



void Client::loadDataHeader(Receive *receiver)
{
	std::string path;
	switch (this->matchingLocation())
	{
		case NO_LOCATION:
			path = this->request.getPath();
			path = this->server->getRoot() + path;
			if (isDirPermissions(path, F_OK | R_OK) == true)
				path += this->server->getIndex();
			this->request.setPath(path);
			this->fileContent->setAutoIndex(server->getAutoIndex());
			break ;
		case NOT_ALLOWED :
			return ;
		case REDIRECT:
			return ;
	}

	this->request.setPath(decodeURL(this->request.getPath()));

	std::cout << request.getPath() << std::endl;
	// std::cout << "FILESIZE " << getFileSize(this->request.getPath()) << std::endl;
	// std::cout << "MAXFILESIZE " << this->server->getMaxClientBodySize() << std::endl;
	if (getFileSize(this->request.getPath()) > this->server->getMaxClientBodySize())
	{
		response.setStatus("413 Request Entity Too Large");
		return;
	}
	if (this->request.getMethod() == "GET")
	{
		if (getMimeType(this->request.getPath()).find("video") != std::string::npos)
		{
			this->fileContent->setRange(stringToSizeT(request.getAttribute("Range")));
			if (this->fileContent->setFileName(this->request.getPath()))
			{
				response.setStatus("206 Partial Content");
				response.setContentType(this->request.getPath());
				response.setLastModified(this->fileContent->getLastModified());
				response.setContentLength(this->fileContent->getContentSize() - stringToSizeT(request.getAttribute("Range")));
				response.setAttribute("Content-Range", "bytes 0-" + toString(this->fileContent->getContentSize() - 1) + "/" + toString(this->fileContent->getContentSize()) );
				response.setServer(server->getServerName());
			}
			else
				response.setStatus("404 Not Found");
		}
		if (this->fileContent->setFileName(this->request.getPath()))
		{
			response.setContentType(this->request.getPath());
			response.setLastModified(this->fileContent->getLastModified());
			response.setContentLength(this->fileContent->getContentSize());
			response.setStatus("200 OK");
			response.setServer(server->getServerName());
		}
		else
			response.setStatus("404 Not Found");
	}
	else if (this->request.getMethod() == "POST")
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
					std::string path = this->request.getPath() + "/" + filename;
					if (access(path.c_str(), F_OK) == 0)
					{
						response.setStatus("403 Forbidden");
						return;
					}
					std::fstream file(path.c_str(), std::ios::out | std::ios::binary | std::ios::app);
					file.write(body.c_str(), body.size());
					file.close();
					response.setStatus("201 Created");
					return;
				}
			}
		}
		else if (receiver->getisform())
		{
			response.setStatus("201 Created");
		}
		else
			std::cout << "form: " << receiver->getBody() << std::endl;
		response.setServer(server->getServerName());
	}
	else if (this->request.getMethod() == "DELETE")
	{
		if (std::remove(this->request.getPath().c_str()) == 0)
			response.setStatus("200 OK");
		else
			response.setStatus("404 Not Found");
		response.setServer(server->getServerName());
	}
	else
	{
		response.setStatus("405 Method Not Allowed");
		response.setServer(server->getServerName());
	}
}
