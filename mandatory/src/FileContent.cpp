#include "../inc/FileContent.hpp"

FileContent::FileContent()
{
	fileName = "";
	sendComplete = false;
	isFileOpen = false;
}
FileContent::FileContent(const std::string &MyfileName) 
{
	this->setFileName(MyfileName);
	sendComplete = false;
	isFileOpen = false;
}

FileContent::~FileContent() {}

int FileContent::openFile()
{
	std::cout << "Opening file " << fileName << std::endl;
	file.open(fileName.c_str());

	if (file.is_open())
		return 1;
	return 0;
}

std::string FileContent::getContent() 
{
	std::string errorReturn = "Error: " + fileName + " File not found";
	if (!isFileOpen)
	{
		std::string line;
		while (std::getline(file, line))
		{
			content += line + "\n";
			if (content.size() >= MAX_SENT_BYTES)
				return content;
		}
		file.close();
		std::cout << "File read: " << fileName << std::endl;
	}
	else
	{
		std::cout << CHR_RED + errorReturn + RESET << std::endl;
		return (errorReturn);
	}
	sendComplete = true;
	return content;
}

void FileContent::setFileName(const std::string &file_name)
{
	if (file_name.find("?") != std::string::npos)
		fileName = file_name.substr(0, file_name.find("?"));
	else
		fileName = file_name;
	std::cout << "File name set to: " << fileName << std::endl;
	isFileOpen = this->openFile();
}

std::string FileContent::getFileName()
{
	return fileName;
}

bool FileContent::isSendComplete()
{
	return sendComplete;
}
