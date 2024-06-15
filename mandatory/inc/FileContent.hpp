#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <time.h>
#include <filesystem>
#include <vector>
#include <map>
#include "../inc/colors.h"
#include "../inc/CGI.hpp"
#include "../inc/Server.hpp"

#define MAX_SENT_BYTES 8192

class Client;
#include "../inc/Client.hpp"

class FileContent
{
private:
	std::string fileName;
	std::vector <std::string> args;
	std::string content;
	std::ifstream file;
	bool isFistFragment;
	bool isFileOpen;
	bool sendComplete;
	struct stat fileStat;
	CGI *cgiModule;
	Server *server;

public:
	FileContent(Server *);
	FileContent(std::string &, Server *);
	~FileContent();
	int openFile();
	bool setFileName(std::string &);
	std::string getFileName();
	std::string getContent();
	bool isSendComplete();
	void setFirstFragment(bool);
	bool getFirstFragment();
	std::string getLastModified();
	size_t getContentSize();
	bool isCGIFile();
	void setCGIFile(bool);
	void setCGIFolder(const std::string &);
	std::string getCGIFolder();
	std::string getFileExtension();
};
