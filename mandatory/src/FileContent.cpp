#include "../inc/FileContent.hpp"

FileContent::FileContent(Server *srv)
{
	server = srv;
	sendComplete = false;
	isFileOpen = false;
	isFistFragment = true;
	this->cgiModule = srv->cgiModuleClone();
	fileName = "";
	if(server->getAutoIndex())
	{
		listDir = new ListDir(srv->getRoot());
		isFileOpen = true;
	}
	else
	{
		listDir = nullptr;
	}
}
FileContent::FileContent(const std::string &MyfileName, Server *srv) 
{
	server = srv;
	sendComplete = false;
	isFistFragment = true;
	this->cgiModule = srv->cgiModuleClone();
	if(server->getAutoIndex())
	{
		fileName = MyfileName;
		listDir = new ListDir(MyfileName);
		isFileOpen = true;
	}
	else
	{
		listDir = nullptr;
		isFileOpen = this->setFileName(MyfileName);
	}
}

FileContent::~FileContent() 
{
	delete cgiModule;
	if (listDir)
		delete listDir;
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
		else if (server->getAutoIndex() && this->fileStat.st_mode & S_IFDIR)
		{
			std::cout << "is an autoindex file: " << fileName << std::endl;
			content = listDir->getContentToList();
			sendComplete = listDir->getsIsSendComlete();
			return content;
		}
		else
		{
			std::cout << "is a normal file: " << fileName << std::endl;
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

bool FileContent::setFileName(const std::string &file_name)
{
	std::string tmp = file_name.substr(0, file_name.find("?"));
	bool filefound = false;
	if (stat(tmp.c_str(), &fileStat) == 0)
	{
		filefound = true;
	}
	else
	{
		std::cout << "File <" << file_name << "> not found: " << filefound << std::endl;
		return false;
	}
	if (cgiModule->setIsCGI(file_name))
	{
		cgiModule->setFileName(file_name);
		isFileOpen = true;
	}
	else
	{
		if(stat(fileName.c_str(), &fileStat))
		{
			fileName = file_name;
			isFileOpen = this->openFile();
		}

	}
	if (!isFileOpen)
	{
		std::cerr << "File not open: " << fileName << std::endl;
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
