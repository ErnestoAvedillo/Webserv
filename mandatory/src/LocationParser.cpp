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

bool LocationParser::getIsAutoIndex()
{
	return this->isAutoIndex;
}

bool LocationParser::getIsCGI()
{
	return this->isCGI;
}

size_t LocationParser::getStartRange()
{
	return this->startRange;
}

size_t LocationParser::getEndRange()
{
	return this->endRange;
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
		if ((this->request.getPath().find(locations[i]->getName()) != std::string::npos && locations[i]->getLocationType() != RETURN )|| (locations[i]->getLocationType() == RETURN && this->request.getPath() == locations[i]->getName()))
		{
			if (isAllowedMethod(locations[i]) == NOT_ALLOWED)
				return NOT_ALLOWED;
			
			std::string rawPath = this->request.getPath();
			this->isAutoIndex = locations[i]->getAutoIndex();
			if ((this->isCookie = locations[i]->getIsCookie()) == true)
				this->cookies = locations[i]->getCookies();
			if (locations[i]->getIsCgi() == true)
			{
				std::string extension;
				if (rawPath.rfind(".") != std::string::npos)
					extension = rawPath.substr(rawPath.rfind(".") + 1, rawPath.size());
				for (size_t y = 0; y < locations[i]->getCgiExtension().size(); y++)
				{

					if (locations[i]->getCgiExtension()[y] == extension)
						this->isCGI = true;
				}		
			}
			ExtendedString tmp;
			switch (locations[i]->getLocationType())
			{
				case RETURN:
						response.setStatus("301 Moved Permanently");
						response.setAttribute("Location", locations[i]->getReturn());
						return REDIRECT;
				case ALIAS:
					tmp = this->request.getPath();
					tmp.replaceFirstString(locations[i]->getName(), locations[i]->getAlias() + "/");
					if (!locations[i]->getIndex().empty() && isDirPermissions(tmp, F_OK | R_OK) == true && this->request.getMethod() != "POST")
						tmp += "/" + locations[i]->getIndex();
					request.setPath(tmp);
					break ;
				case ROOT:
					tmp = this->request.getPath();
					tmp.replaceFirstString(locations[i]->getName(), locations[i]->getRoot()  + locations[i]->getName());
					if (!locations[i]->getIndex().empty() && isDirPermissions(tmp, F_OK | R_OK) == true && this->request.getMethod() != "POST")
						tmp += "/" + locations[i]->getIndex();
					request.setPath(tmp);
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
            default:
                escapedStr += str[i];
                break;
        }
    }
    return escapedStr;
}

bool isBadRequest(std::string request)
{
	if (request.empty())
        return true;
	std::istringstream iss(request);
	std::string line;
	bool firstLine = true;

    while (std::getline(iss, line))
	{   
		if (firstLine)
		{
			if (std::count(line.begin(), line.end(), ' ') != 2 && splitString(line, ' ').size() != 3)
				return true;
			firstLine = false;
		}
		else 
		{
			if (line.empty() || line == "\r")
					break ;
			
			if (line.find(':') == std::string::npos)
				return true;
		}
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

void LocationParser::setCookies()
{
	if (isCookie)
	{
		std::string setcookie;
		std::vector<std::string> querylines = splitString(this->query, '&');
		for (size_t i = 0; i < querylines.size(); i++)
		{
			std::vector<std::string> queryline = splitString(querylines[i], '=');
			if (std::find(this->cookies.begin(), this->cookies.end(), queryline[0]) != this->cookies.end())
				response.setCookie(queryline[0] + "=" + queryline[1] + "; path=/");
		}	
	}
}

void LocationParser::checks()
{
	std::string path;
	if (this->request.getPath().find("?") != std::string::npos)
	{
		this->query = this->request.getPath().substr(this->request.getPath().find("?") + 1);
		this->query = decodeURL(this->query);
		this->request.setPath(this->request.getPath().substr(0, this->request.getPath().find("?")));
	}
	switch (this->matchingLocation())
	{
		case NO_LOCATION:
			path = this->request.getPath();
			path = this->server->getRoot() + path;
			if (isDirPermissions(path, F_OK | R_OK) == true)
				path += this->server->getIndex();
			this->request.setPath(path);
			break ;
		case NOT_ALLOWED :
			response.setStatus("405 Method Not Allowed");
			throw METHOD_NOT_ALLOWED_CODE;
			return ;
		case REDIRECT:
			return ;
	}

	this->request.setPath(decodeURL(this->request.getPath()));
	if (isBadRequest(receiver->getRequest()))//|| isURIMalformed(this->request.getPath())
	{
		response.setStatus("400 Bad Request");
		throw BAD_REQUEST_CODE;
		return;
	}
	if (!isMethodImplemented(this->request.getMethod()))
	{
		response.setStatus("501 Not Implemented");
		throw NOT_IMPLEMENTED_CODE;
		return;
	}
	if (isMethodNotStandard(this->request.getMethod()))
	{
		response.setStatus("405 Method Not Allowed");
		throw METHOD_NOT_ALLOWED_CODE;
		return;
	}
	if (!isVersionImplemented(this->request.getProtocol()))
	{
		response.setStatus("505 HTTP Version Not Supported");
		throw HTTP_VERSION_NOT_SUPPORTED_CODE;
		return;
	}	
	if (this->request.getMethod() == "GET")
	{
		if (getMimeType(this->request.getPath()).find("video") != std::string::npos)
		{
			// std::string attr = request.getAttribute("Range");
			// int tt = isFilePermissions(this->request.getPath(), F_OK | R_OK);
			// (void)tt;
			// if (isFilePermissions(this->request.getPath(), F_OK | R_OK) == 1 && request.getAttribute("Range") != "")
			// {
			
			// 	response.setStatus("206 Partial Content");
			// 	response.setContentType(this->request.getPath());
				
			// 	if (this->request.getAttribute("Range") != "")
			// 	{
			// 		this->startRange = stringToSizeT(this->request.getAttribute("Range").substr(6, this->request.getAttribute("Range").find("-")));
			// 		std::cout << "STARTRANGE " << this->startRange << std::endl;
			// 		std::string endRangeStr = this->request.getAttribute("Range").substr(this->request.getAttribute("Range").find("-") + 1, this->request.getAttribute("Range").size());
			// 		std::cout << "ENDRANGE " << endRangeStr << std::endl;
			// 		if (endRangeStr.empty())
			// 		{
			// 			this->endRange = getFileSize(this->request.getPath());
			// 		}
			// 		else
			// 		{
			// 			this->endRange = stringToSizeT(endRangeStr);
			// 		}
			// 	}
			// }
			if (isFilePermissions(this->request.getPath(), F_OK | R_OK) == 1)
			{
				response.setContentType(this->request.getPath());
				response.setContentLength(getFileSize(this->request.getPath()));
				response.setStatus("200 OK");
			}
			else
			{
				response.setStatus("404 Not Found");
				throw NOT_FOUND_CODE;
			}
		}
		else if (isFilePermissions(this->request.getPath(), F_OK | R_OK) == 1)
		{
			response.setContentType(this->request.getPath());
			response.setStatus("200 OK");
		}
		else if (isDirPermissions(this->request.getPath(), F_OK | R_OK) == true && this->isAutoIndex == true)
		{
			response.setContentType("text/html");
			response.setStatus("200 OK");
		}
		else
		{
			response.setStatus("404 Not Found");
			throw NOT_FOUND_CODE;
		}
	}
	else if (this->request.getMethod() == "POST")
	{
		
		std::string body = receiver->getBody();
		if (body.size() > (size_t)this->server->getMaxClientBodySize())
		{
			response.setStatus("413 Request Entity Too Large");
			throw REQUEST_ENTITY_TOO_LARGE_CODE;
			return;
		}
		if (this->request.getMethod() == "POST" && receiver->getisform() == false)
		{
			if (receiver->getBody().empty())
			{
				response.setStatus("400 Bad Request");
				throw 400;
				return ;
			}
			std::string postheader = receiver->getPostHeader();
			if (this->request.getAttribute("Content-Length") == "")
			{
				response.setStatus("411 Length Required");
				throw LENGTH_REQUIRED_CODE;
				return ;
			}
			std::vector<std::string> lines = splitString(postheader, '\n');
			for (size_t i = 0; i < lines.size(); i++)
			{
				if (lines[i].find("filename=") != std::string::npos)
				{
					std::cout << "filename: " << lines[i] << std::endl;
					std::string filename = lines[i].substr(lines[i].find("filename=") + 10, lines[i].size());
					std::string path = this->request.getPath();
					filename = filename.substr(0, filename.find("\""));
					if (path.find(filename) == std::string::npos)
					{
						path +=  "/" + filename;	
					}
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
			this->query = decodeURL(body);
			std::cout << this->query << std::endl;

			response.setStatus("201 Created");
			throw CREATED_CODE;
		}
		response.setServer(server->getServerName());
	}
	else if (this->request.getMethod() == "DELETE")
	{
		if (std::remove(this->request.getPath().c_str()) == 0)
			response.setStatus("200 OK");
		else
		{
			response.setStatus("404 Not Found");
			throw NOT_FOUND_CODE;
		}
	}
	else
	{
		response.setStatus("405 Method Not Allowed");
		throw METHOD_NOT_ALLOWED_CODE;
	}
}

LocationParser::LocationParser(Header request_, Server *server_, Receive *receiver_)
{
	this->request = request_;
	this->receiver = receiver_;
	this->server = server_;
	this->isCGI = false;
	this->startRange = 0;
	this->endRange = 0;
	this->isAutoIndex = server->getAutoIndex();
	this->startRange = 0;
	this->endRange = 0;
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

std::string LocationParser::getQuery()
{
	return this->query;
}