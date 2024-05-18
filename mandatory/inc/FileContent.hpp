#pragma once
#include <iostream>
#include <fstream>
#include <string>

class FileContent
{
private:
	std::string fileName;
	std::string content;

public:
	FileContent();
	FileContent(const std::string &);
	~FileContent();
	int openFile();
	void setFileName(const std::string &);
	std::string getFileName();
	std::string getContent();
};
