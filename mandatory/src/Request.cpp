#include "../inc/Request.hpp"

Request::Request()
{
}

Request::Request(std::string const &str)
{
	this->loadCompleteRequest(str);
}

Request &Request::operator=(Request const &rsh)
{
	this->request = rsh.request;
	this->header = rsh.header;
	return (*this);
}

Request::~Request()
{
}

void Request::addRequest(std::string const &key, std::string const &value)
{
	this->request[key] = value;
}

std::map<std::string, std::string>::iterator Request::findRequest(std::string const &key)
{
	return (this->request.find(key));
}

std::map<std::string, std::string>::iterator Request::getBeginRequest()
{
	return (this->request.begin());
}

std::map<std::string, std::string>::iterator Request::getEndRequest()
{
	return (this->request.end());
}

std::string Request::getHeader()
{
	return (this->header.getHeader());
}

std::string Request::getFileName()
{
	return (this->header.getFileName());
}

void Request::clearRequest()
{
	this->request.clear();
}

void Request::deleteRequest(std::string const &key)
{
	this->request.erase(key);
}

void Request::updateRequest(std::string const &key, std::string const &value)
{
	this->request[key] = value;
}

void Request::loadCompleteRequest(const std::string &requestHeader)
{
	std::vector<std::string> lines = splitString(requestHeader, '\n');
	this->header.setHeader(lines[0]);
	for (size_t i = 1; i < lines.size(); i++)
	{
		std::vector<std::string> parts = splitString(lines[i], ':');
		if (parts.size() == 2)
			this->addRequest(parts[0], parts[1]);
	}
}