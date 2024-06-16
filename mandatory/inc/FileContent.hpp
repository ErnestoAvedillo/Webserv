#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <time.h>
#include <filesystem>
#include <vector>
#include <map>
#include <vector>
#include <map>
#include "colors.h"
#include "CGI.hpp"
#include "Server.hpp"
#include "ListDir.hpp"
#include "ExtendedString.hpp"
#include "StateCode.hpp"
//#define MAX_SENT_BYTES 8192

class Client;
#include "../inc/Client.hpp"

class FileContent
{
private:
	std::string fileName;
	std::vector <std::string> args;
	ExtendedString content;
	std::ifstream file;
	bool isFistFragment;
	bool isFileOpen;
	bool sendComplete;
	struct stat fileStat;
	CGI *cgiModule;
	Server *server;
	ListDir *listDir;
	StateCode *stateCode;

	size_t completeContentSize;
	std::string splitFileFromArgs(const std::string &);
	bool FileOrFolerExtists(const std::string &);
	bool isInputDirectory();
	size_t startRange;
	bool isAutoIndex;
	bool isCgi;
public:
	FileContent(Server *);
	FileContent(const std::string &, Server *);
	~FileContent();
	int openFile();
	bool setFileName(const std::string &);
	void setIsCGI(bool isCgi);
	std::string getFileName();
	std::string getContent();
	void setIsSendComplete(bool);
	bool isSendComplete();
	void setFirstFragment(bool);
	bool getFirstFragment();
	std::string getLastModified();
	size_t getContentSize();
	void setRange(size_t );
	void setIsAutoIndex(bool autoIndex);
};
