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
// #include "Server.hpp"
#include "ListDir.hpp"
#include "ExtendedString.hpp"
#include "StatusCode.hpp"

//#define MAX_SENT_BYTES 8192
class FileContent : public StatusCode
{
private:
	std::string fileName;
	std::string indexName;
	std::vector <std::string> args;
	ExtendedString content;
	std::ifstream file;
	bool isFistFragment;
	bool isFileOpen;
	bool sendComplete;
	bool isAutoIndex;
	std::string homeFolder;
	struct stat fileStat;
	CGI *cgiModule;
	ListDir *listDir;
	//Server *server;
	size_t completeContentSize;
	std::string splitFileFromArgs(const std::string &);
	bool FileOrFolerExtists(const std::string &);
	size_t startRange;
//	bool isCgi;
public:
	// FileContent(Server *);
	FileContent();
	// FileContent(const std::string &, Server *);
	~FileContent();
	int openFile();
	bool setFileName(const std::string &);
	void setIsCGI(bool isCgi);
	std::string getFileName();
	std::string getContent(size_t);
	void setIsFileOpen(bool);
	void setIsSendComplete(bool);
	bool getIsSendComplete();
	bool getIsFileOpen();
	void setFirstFragment(bool);
	bool getFirstFragment();
	std::string getLastModified();
	size_t getCompleteContentSize();
	void setCompleteContentSize(size_t);
	bool isInputDirectory();
	void setRange(size_t );
	void setIsAutoIndex(bool);
	bool getIsAutoIndex();
	void setIndexName(const std::string &);
	std::string getIndexName();
	void setHomeFolder(const std::string &);
	std::string getHomeFolder();
	void setCGIModule(CGI *);
};
