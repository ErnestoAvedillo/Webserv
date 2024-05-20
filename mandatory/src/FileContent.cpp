#include "../inc/FileContent.hpp"

FileContent::FileContent()
{
	fileName = "";
	sendComplete = false;
	isFileOpen = false;
	isFistFragment = true;
}
FileContent::FileContent(const std::string &MyfileName) 
{
	if (stat(fileName.c_str(), &fileStat) < 0)
			isFileOpen = false;
	else
	{
		this->setFileName(MyfileName);
		isFileOpen = true;
	}
	sendComplete = false;
	isFistFragment = true;
}

FileContent::~FileContent() {}

int FileContent::openFile()
{
	std::cout << "Opening file " << fileName << std::endl;
	file.open(fileName.c_str(), std::ios::out | std::ios::binary); //| std::ios::app | std::ios::ate

	if (file.is_open())
		return 1;
	return 0;
}

std::string FileContent::getContent() 
{
	std::string errorReturn = "Error: " + fileName + " File not found";
	if (isFileOpen)
	{
		content = "";
		char buffer[MAX_SENT_BYTES];
		if(file.read(buffer, MAX_SENT_BYTES))
		{
			content.append(buffer, file.gcount());
			return content;
		}
		else
		{
			std::cout << "File closed: " << fileName << std::endl;
			file.close();
			content.append(buffer, file.gcount());
		}
	}
	else
	{
		content = errorReturn;
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

void FileContent::setFirstFragment(bool first)
{
	isFistFragment = first;
}

bool FileContent::getFirstFragment()
{
	return isFistFragment;
}

std::string FileContent::getLastModified()
{
	if (stat(fileName.c_str(), &fileStat) < 0)
	{
		return "";
	}
	return ctime(&fileStat.st_mtime);
}

size_t FileContent::getContentSize()
{
	return fileStat.st_size;
}
