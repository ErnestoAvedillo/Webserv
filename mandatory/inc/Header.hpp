#pragma once
#include <string>
#include <map>
#include <iostream>
#include <ctime>
#include "../inc/utils.hpp"
#include <algorithm>
#include "../inc/ExtendedString.hpp"

class Header 
{
	private:
		/* Receive Header*/
		std::string method;
		ExtendedString path;
		
		std::string protocol;
		std::string status;
		std::string server;
		std::string date;
		std::string lastModified;
		size_t contentLength;
		std::string contentType;
		std::map<std::string, std::string> attributes;

public:
	Header();
	Header(std::string receiveHeader);
	~Header();
	std::string generateHeader() const;
	void setVersion(std::string version);
	void setStatus(std::string status);
	void setServer(std::string server);
	void setDate();
	void setLastModified(std::string lastModified);
	void setContentLength(size_t contentLength);
	void setContentType(std::string contentType);
	void setAttribute(std::string key, std::string value);
	void setMethod(std::string method);
	void setPath(std::string path);
	void setProtocol(std::string protocol);
	std::string getMethod();
	ExtendedString getPath();
	std::map<std::string, std::string> getAttributes();
	std::string getContentType();
	std::string getAttribute(std::string key);
};
