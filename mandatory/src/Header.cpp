
#include "../inc/Header.hpp"

Header::Header()
{
	Version = "1.1 ";
	ContentLength = 0;
	ContentType = "text/html";
	this->setDate();
}

Header::~Header(){}

std::string Header::generateHeader() const
{
	std::string header;
	
	header = "HTTP/" + Version + " " + Status + "\r\n"; 
	header += "Server: " + Server + "\r\n";
	header += "Date: " + Date + "\r\n";
	if (LastModified != "")
		header += "Last-modified: " + LastModified + "\r\n";
	if(ContentLength != 0)
		header += "Content-length: " + std::to_string(ContentLength) + "\r\n";
	header += "Content-Type: " + ContentType + "\r\n";
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
	std::time_t currentTime = std::time(nullptr);

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
	ContentType = contentType;
}
