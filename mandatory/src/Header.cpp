
#include "../inc/Header.hpp"

Header::Header()
{
	Version = "1.1";
	ContentLength = 0;
	ContentType = "text/html";
	this->setDate();
}


Header::Header(std::string receiveHeader)
{
	std::istringstream iss(receiveHeader);
	std::string line;
	while (std::getline(iss, line))
	{
		size_t colonPos = line.find(':');
		if (colonPos != std::string::npos)
		{
			std::string key = line.substr(0, colonPos);
			std::string value = line.substr(colonPos + 1);
			key.erase(0, key.find_first_not_of(' '));
			key.erase(key.find_last_not_of(' ') + 1);
			value.erase(0, value.find_first_not_of(' '));
			value.erase(value.find_last_not_of(' '));
			this->attributes[key] = value;
		}
		else
		{
			if (std::count(line.begin(), line.end(), ' ') == 2)
			{
				size_t firstSpace = line.find(' ');
				size_t secondSpace = line.find(' ', firstSpace + 1);
				method = line.substr(0, firstSpace);
				path = line.substr(firstSpace + 1, secondSpace - firstSpace - 1);
				protocol = line.substr(secondSpace + 1);

			}
		}
	}
}

void Header::setAttribute(std::string key, std::string value)
{
	this->attributes[key] = value;
}

std::map<std::string, std::string> Header::getAttributes()
{
	return attributes;
}
Header::~Header(){}

std::string Header::generateHeader() const
{
	std::string header;
	
	// std::cout << "Version: $" << Version << "$" << std::endl;
	header = protocol + " " + status + "\r\n"; 
	header += "Server: " + server + "\r\n";
	header += "Date: " + date + "\r\n";
	if (lastModified != "")
		header += "Last-modified: " + lastModified + "\r\n";
	if(contentLength != 0)
		header += "Content-length: " + toString(contentLength) + "\r\n";
	header += "Content-Type: " + contentType + "\r\n";
	for (std::map<std::string, std::string>::const_iterator it = attributes.begin(); it != attributes.end(); ++it)
		header += it->first + ": " + it->second + "\r\n";
	
	header += "\r\n";
	
	return header;
}

void Header::setVersion(std::string version)
{
	Version = version;
}

void Header::setStatus(std::string status)
{
	Status = status;
}

void Header::setServer(std::string server)
{
	Server = server;
}

void Header::setDate()
{
	// Get the current time
	std::time_t currentTime = std::time(NULL);

	// Convert the current time to a string in the desired format
	std::tm* timeInfo = std::gmtime(&currentTime);
	char buffer[80];
	std::strftime(buffer, sizeof(buffer), "%A, %d-%b-%y %H:%M:%S GMT", timeInfo);

	// Set the date in the header
	Date = buffer;
}

void Header::setLastModified(std::string lastModified)
{
	LastModified = lastModified;
}

void Header::setContentLength(size_t contentLength)
{
	ContentLength = contentLength;
}

void Header::setContentType(std::string contentType)
{
	size_t point = contentType.find_last_of(".");
	std::string extension = contentType.substr(point + 1, contentType.size());

	/* Create once only */
	std::map<std::string, std::string> Mimetype = create_filetypes();
	if (Mimetype.find(extension) != Mimetype.end())
		ContentType = Mimetype[extension];
	else
		ContentType = "text/html";

}