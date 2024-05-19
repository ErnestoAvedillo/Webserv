#pragma once
#include <iostream>
#include <fstream>
#include <string>
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

public:
	FileContent();
	FileContent(const std::string &);
	~FileContent();
	int openFile();
	void setFileName(const std::string &);
	std::string getFileName();
	std::string getContent();
	bool isSendComplete();
	void setFirstFragment(bool);
	bool getFirstFragment();
};
