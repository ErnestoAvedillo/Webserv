#include "../inc/FileContent.hpp"

FileContent::FileContent(Server *srv)
{
	server = srv;
	fileName = "";
	sendComplete = false;
	isFileOpen = false;
	isFistFragment = true;
	this->cgiModule = srv->cgiModuleClone();
}
FileContent::FileContent(const std::string &MyfileName, Server *srv) 
{
	server = srv;
	isFileOpen = this->setFileName(MyfileName);
	sendComplete = false;
	isFistFragment = true;
	this->cgiModule = srv->cgiModuleClone();
}

FileContent::~FileContent() 
{
	delete cgiModule;
}

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
		if (cgiModule->getIsCGI())
		{
			std::cout << "is a CGI file: " << fileName << std::endl;
			sendComplete = true;
			return cgiModule->execute();
		}
		else
		{
			content = "";
			char buffer[MAX_SENT_BYTES];
			if(file.read(buffer, MAX_SENT_BYTES))
			{
				if(file.eof())
				{
					file.close();
					sendComplete = true;
				}
				content.append(buffer, file.gcount());
				return content;
			}
			else
			{
				file.close();
				content.append(buffer, file.gcount());
			}
		}
	}
	else
	{
		content = errorReturn;
	}
	sendComplete = true;
	return content;
}

bool FileContent::setFileName(std::string &file_name)
{
	std::string tmp = file_name.substr(0, file_name.find("?"));

	if (access(tmp.c_str(), F_OK) == 0)
	{
		file_name = tmp;
	}
	else
		return false;
	if (cgiModule->setIsCGI(file_name))
	{
		cgiModule->setFileName(file_name);
		isFileOpen = true;
	}
	else
	{
		if (access(tmp.c_str(), F_OK) == 0)
		{
			fileName = file_name;
			isFileOpen = this->openFile();
		}

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
