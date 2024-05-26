#include "../inc/FileContent.hpp"

FileContent::FileContent()
{
	fileName = "";
	sendComplete = false;
	isFileOpen = false;
	isFistFragment = true;
	isCGI = false;
	this->cgiModule = new CGI();
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
			if(file.eof())
			{
				std::cout << "File closed: " << fileName << std::endl;
				file.close();
				sendComplete = true;
			}
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

bool FileContent::setFileName(const std::string &file_name)
{
	if (file_name.find("?") != std::string::npos)
		fileName = file_name.substr(0, file_name.find("?"));
	else
		fileName = file_name;
	
	isFileOpen = this->openFile();
	if (isFileOpen)
	{
		std::cout << "File open: " << fileName << std::endl;
	}
	else
	{
		std::cout << "File not open: " << fileName << std::endl;
	}
	return isFileOpen;
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
	char buffer[80];

	strftime(buffer, sizeof(buffer), "%A, %d-%b-%y %H:%M:%S GMT", localtime(&fileStat.st_mtime));
	return buffer;
}

size_t FileContent::getContentSize()
{
	return fileStat.st_size;
}

bool FileContent::isCGIFile()
{
	return isCGI;
}

void FileContent::setCGIFile(bool isCGI)
{
	this->isCGI = isCGI;
}

