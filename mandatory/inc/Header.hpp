#pragma once
#include <string>
#include <map>
#include <iostream>
#include <ctime>
#include "../inc/utils.hpp"
#include <algorithm>


class Header 
{
	private:
		/* Receive Header*/
		std::string method;
		std::string path;
		
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
	std::string getMethod();
	std::string getPath();
	std::map<std::string, std::string> getAttributes();
	std::string getContentType();

};
