#include "../inc/Request.hpp"

/* Default Constructor */
Request::Request()
{
	std::cout << "Default Constructor called" << std::endl;	
}

/* Load Constructor */
Request::Request(std::string &requestHeader)
{
	std::cout << "Load Constructor called" << std::endl;	
	this->loadCompleteRequest(requestHeader);
}

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


void Request::loadLine(std::string requestLine)
{
	std::string	token;
	std::stringstream ss(requestLine);
	std::string	key;
	std::string value;

	if (requestLine.length() == 0)
		return ;
	requestLine.pop_back();
	if (requestLine.length() == 0)
		return ;
	size_t pos1 = requestLine.find(":",0);
	key = requestLine.substr(0, pos1);
	value =  requestLine.substr(pos1 + 1, requestLine.length() - pos1 - 1);
	this->Header[key] = value;
}


void Request::loadCompleteRequest(std::string requestHeader)
{
	std::string token;
	std::stringstream ss(requestHeader);

	std::getline(ss, token, '\n');
	this->loadRequestLine(token);
	while (std::getline(ss, token, '\n'))
		this->loadLine(token);

}

/* @brief to load the first line of an http header or Request Line Attributes*/
void Request::loadRequestLine(std::string requestHeader)
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
