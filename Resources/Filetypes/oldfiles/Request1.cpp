#include "../inc/Request.hpp"

/* Default Constructor */
Request::Request()
{
	std::cout << "Default Constructor called" << std::endl;	
}

/* Load Constructor */
// Request::Request(std::string &requestHeader)
// {
// 	std::cout << "Load Constructor called" << std::endl;	
// 	//this->loadRequestLine(requestHeader);
// 	(void)requestHeader;
// }

/* Copy Constructor */
Request::Request(const Request &copy)
{
	std::cout << "Copy Constructor called" << std::endl;	
	*this = copy;
}

/* Copy Assignment operator */
Request& Request::operator=(const Request &copy)
{
	std::cout << "Copy Assignment operator called" << std::endl;
	if (this != &copy)
	{
		this->Method = copy.Method;
		this->Path = copy.Path;
		this->Protocol = copy.Protocol;
		this->Header = copy.Header;
	}
	return (*this);	
}

/* Destructor */
Request::~Request()
{
	std::cout << "Destructor called" << std::endl;	
}

std::string Request::getMethod() const
{
	return(this->Method);
}

std::string Request::getPath() const
{
	return(this->Path);
}
std::string Request::getProtocol() const
{
	return(this->Protocol);
}

std::map<std::string, std::string> Request::getHeader() const
{
	return (this->Header);
}


void Request::loadLine(const std::string &requestLine)
{
	std::string	token;
	std::stringstream ss(requestLine);
	std::string	key;
	std::string value;

	// for (int i = 0; i < 2; i++)
	// {
	// 	std::getline(ss, token, ':');
	size_t pos1 = requestLine.find(":",0);
	//size_t pos2 = requestLine.find(_delimiter,pos1 + 1) ;
	//_list_data.insert(std::pair<std::string, std::string>(requestLine.substr(0, pos1), requestLine.substr(pos1 + 1,)));
	key = requestLine.substr(0, pos1);
	value =  requestLine.substr(pos1 + 1, requestLine.length() - pos1 - 1);
	this->Header[key] = value;
}


void Request::loadCompleteRequest(const std::string &requestHeader)
{
	std::string token;
	std::stringstream ss(requestHeader);
	
	// if (countChar(requestHeader, ':') > 0)
	// {
	// 	std::cout << requestHeader << std::endl;
	//std::getline(ss, token, '\n');
	std::getline(ss, token, '\n');
	this->loadRequestLine(token);
	while (std::getline(ss, token, '\n'))
		this->loadLine(token);

}

/* @brief to load the first line of an http header or Request Line Attributes*/
void Request::loadRequestLine(const std::string requestHeader)
{
	std::string token;
	std::stringstream ss(requestHeader);
	
	for (int i = 0; i < 3; i++)
	{
		std::getline(ss, token, ' ');
		switch (i)
		{
			case 0:
				this->Method = token;
				break ;
			case 1:
				this->Path = token;
				break ;
			case 2:
				this->Protocol = token;
				if (this->Protocol.back() == '\n')
					this->Protocol.pop_back();
				break ;
		}	
	}
}
