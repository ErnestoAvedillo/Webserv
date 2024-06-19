#include "../inc/LocationParser.hpp"

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

int LocationParser::isAllowedMethod(Location *location)
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


int LocationParser::matchingLocation()
{
	std::vector<Location *> locations = this->server->getLocations();
	for (size_t i = 0; i < locations.size(); i++)
	{
		if (this->request.getPath().find(locations[i]->getName()) != std::string::npos)
		{
			if (isAllowedMethod(locations[i]) == NOT_ALLOWED)
				return NOT_ALLOWED;
			// this->fileContent->setAutoIndex(locations[i]->getAutoIndex());
			// this->fileContent->setIsCGI(locations[i]->getIsCgi());
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

std::string escapeNonPrintableChars(const std::string& str) {
    std::string escapedStr;
    for (size_t i = 0; i < str.size(); ++i) {
        switch (str[i]) {
            case '\n':
                escapedStr += "\\n";
                break;
            case '\r':
                escapedStr += "\\r";
                break;
            case '\t':
                escapedStr += "\\t";
                break;
            // Add more cases for other non-printable characters if needed
            default:
                escapedStr += str[i];
                break;
        }
    }
    return escapedStr;
}

bool isBadRequest(std::string request)
{
	// std::cout << "REQUEST: " << escapeNonPrintableChars(request) << std::endl;
	std::istringstream iss(request);
	std::string line;

	if (request.empty())
        return true;
	std::getline(iss, line);
	if (std::count(line.begin(), line.end(), ' ') != 2 && splitString(line, ' ').size() != 3)
		return true;
    while (std::getline(iss, line))
	{   
       if (line.empty() || line == "\r")
            break ;
        size_t colonPos = line.find(':');
		if (colonPos == std::string::npos)
			return true;
	}
	return false;
}

bool isMethodImplemented(std::string method)
{
	if	(method == "HEAD" || method == "PUT" || method == "CONNECT" || method == "OPTIONS" || method == "TRACE" || method == "PATCH")
		return false;
	return true;
}

bool isVersionImplemented(std::string version)
{
	if (version == "HTTP/1.1")
		return true;
	return false;
}

bool isMethodNotStandard(std::string method)
{
	if (method != "GET" && method != "POST" && method != "DELETE")
		return true;
	return false;
}


LocationParser::LocationParser(Header request_, Server *server_, Receive *receiver_)
{
	this->request = request_;
	this->receiver = receiver_;
	this->server = server_;
	std::string path;
	switch (this->matchingLocation())
	{
		case NO_LOCATION:
			path = this->request.getPath();
			path = this->server->getRoot() + path;
			if (isDirPermissions(path, F_OK | R_OK) == true)
				path += this->server->getIndex();
			this->request.setPath(path);
			// this->fileContent->setAutoIndex(server->getAutoIndex());
			break ;
		case NOT_ALLOWED :
			return ;
		case REDIRECT:
			return ;
	}
	
	this->request.setPath(this->request.getPath().substr(0, this->request.getPath().find("?")));
	this->request.setPath(decodeURL(this->request.getPath()));
	// std::cout << request.getPath() << std::endl;
	if (isBadRequest(receiver->getRequest()))//|| isURIMalformed(this->request.getPath())
	{
		response.setStatus("400 Bad Request");
		return;
	}
	if (!isMethodImplemented(this->request.getMethod()))
	{
		response.setStatus("501 Not Implemented");
		return;
	}
	if (isMethodNotStandard(this->request.getMethod()))
	{
		response.setStatus("405 Method Not Allowed");
		return;
	}
	if (!isVersionImplemented(this->request.getProtocol()))
	{
		response.setStatus("505 HTTP Version Not Supported");
		return;
	}
	if (getFileSize(this->request.getPath()) > this->server->getMaxClientBodySize())
	{
		response.setStatus("413 Request Entity Too Large");
		return;
	}
	
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
			// this->fileContent->setRange(stringToSizeT(request.getAttribute("Range")));
			if (isFilePermissions(this->request.getPath(), F_OK | R_OK) == 1)
			{
			
				response.setStatus("206 Partial Content");
				response.setContentType(this->request.getPath());
				// response.setLastModified(this->fileContent->getLastModified());
				// response.setContentLength(this->fileContent->getContentSize() - stringToSizeT(request.getAttribute("Range")));
				// response.setAttribute("Content-Range", "bytes 0-" + toString(this->fileContent->getContentSize() - 1) + "/" + toString(this->fileContent->getContentSize()) );
				response.setServer(server->getServerName());
			}
			else
				response.setStatus("404 Not Found");
		}
		if (isFilePermissions(this->request.getPath(), F_OK | R_OK) == 1)
		{
			response.setContentType(this->request.getPath());
			// response.setLastModified(this->fileContent->getLastModified());
			// response.setContentLength(this->fileContent->getContentSize());
			response.setStatus("200 OK");
			response.setServer(server->getServerName());
		}
		else
			response.setStatus("404 Not Found");
	}
	else if (this->request.getMethod() == "POST")
	{
		if (this->request.getMethod() == "POST" && receiver->getisform() == false)
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

LocationParser::~LocationParser()
{


}

Header LocationParser::getResponse()
{
	return this->response;
}

Header LocationParser::getRequest()
{
	return this->request;
}