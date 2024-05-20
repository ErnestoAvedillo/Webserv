#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <filesystem>
#include "../inc/colors.h"

#define MAX_SENT_BYTES 8192
class FileContent
{
private:
	std::string fileName;
	std::string content;
	std::ifstream file;
	bool isFistFragment;
	bool isFileOpen;
	bool sendComplete;
	struct stat fileStat;


public:
	FileContent();
	FileContent(const std::string &);
	~FileContent();
	int openFile();
	bool setFileName(const std::string &);
	std::string getFileName();
	std::string getContent();
	bool isSendComplete();
	void setFirstFragment(bool);
	bool getFirstFragment();
	std::string getLastModified();
	size_t getContentSize();
};
