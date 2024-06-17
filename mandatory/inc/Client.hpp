#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstring>
#include "utils.hpp"
#include <string>
#include <algorithm>

class Server;
#include "../inc/Server.hpp"
#include "../inc/colors.h"

#define REQ_TYPE "Type"
#define REQ_FILE "File"
#define REQ_VER "Version"
#define REQ_CONTENT "Content-Type"

#include "Header.hpp"
#include "Receive.hpp"

class FileContent;
#include "../inc/FileContent.hpp"



enum LocationStatus
{
	/* Correct */
	NO_LOCATION,
	OK,
	NOT_ALLOWED,
	NOT_FOUND,
	NOT_IMPLEMENTED,
	REQUEST_ENTITY_TOO_LARGE,
	REQUEST_URI_TOO_LONG,
	INTERNAL_SERVER_ERROR,
	NOT_MODIFIED,
	REDIRECT,

};

class FileContent;
#include "../inc/FileContent.hpp"
class Client
{
private:
	// std::map<std::string, std::string> Request;
	FileContent *fileContent;
	Server *server;
	Header response;
	Header request;
public:
	Client();
	Client(Receive *, Server *);
	Client(Server *);
	Client &operator=(Client const &);
	~Client();
	void addKeyReq(std::string const &, std::string const &);
	void addKeyType(std::string const &);
	void addKeyFile(std::string const &);
	void addKeyVers(std::string const &);
	// std::map<std::string, std::string>::iterator findClient(std::string const &);
	// std::map<std::string, std::string>::iterator getBeginClient();
	// std::map<std::string, std::string>::iterator getEndClient();
	// void clearClient();
	// void deleteClient(std::string const &);
	// void updateClient(std::string const &, std::string const &);
	void setRequestHeader(std::string );
	// void setResponseHeader(Header);
	void setServer(Server *);
	
	std::string getAnswerToSend();
	std::string getFilePath();
	std::string getFileContent(std::string filename);
	std::string	normalizePath(std::string path);
	std::string getFileContent();
	bool isSendComplete();
	void loadCompleteClient(Receive *receiver, std::vector<Server *> servers);
	void matchServerName(std::vector<Server *> servers);
	void loadDataHeader(Receive *receiver);
	int matchingLocation();
	int isAllowedMethod(Location *location);
};

