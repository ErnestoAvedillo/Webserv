#include "../inc/FileContent.hpp"

FileContent::FileContent(Server *srv)
{
	server = srv;
	fileName = "";
	startRange = 0;
	sendComplete = false;
	isFileOpen = false;
	isFistFragment = true;
	this->cgiModule = srv->cgiModuleClone();
	if(server->getAutoIndex())
	{
		listDir = new ListDir(srv->getRoot());
		isFileOpen = true;
	}
	else
	{
		listDir = NULL;
	}
}

FileContent::FileContent(const std::string &MyfileName, Server *srv) 
{
	server = srv;

	startRange = 0;
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
		listDir = NULL;
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
			sendComplete = true;
			try{
				content = cgiModule->execute();
			}
			catch(const std::exception& e)
			{
				content = e.what();
			}
			completeContentSize = content.size();
			return content;
		}
		else if (server->getAutoIndex() && this->fileStat.st_mode & S_IFDIR)
		{
			content = listDir->getContentToSend();
			completeContentSize = content.size();
			sendComplete = listDir->getIsSendComlete();
			return content;
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

bool FileContent::setFileName(const std::string &file_name)
{
	std::string FileAndFolder = this->splitFileFromArgs(file_name);
	bool fileOrFolderExists = this->FileOrFolerExtists(FileAndFolder);
	if (fileOrFolderExists)
	{
		if (this->isInputDirectory() && this->isAutoIndex)
		{
			fileName = FileAndFolder;
			isFileOpen = true;
			listDir->setSubdirectory(FileAndFolder);
			listDir->setContentToList();
			return isFileOpen;
		}
		//else if (cgiModule->setIsCGI(file_name))
		else if (this->isCgi)
		{
			cgiModule->setFileName(file_name);
			isFileOpen = true;
			return isFileOpen;
		}
		else
		{
			fileName = FileAndFolder;
			stat(fileName.c_str(), &fileStat);
			completeContentSize = fileStat.st_size;
			isFileOpen = this->openFile();
			if (isFileOpen)
				file.seekg(startRange, std::ios::beg);
			return isFileOpen;
		}
	}
	return isFileOpen;
}
void FileContent::setAutoIndex(bool autoIndex)
{
	this->isAutoIndex = autoIndex;
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
	return completeContentSize;
}

bool FileContent::FileOrFolerExtists(const std::string &str)
{
	if (stat(str.c_str(), &fileStat) == 0)
	{
		return true;
	}
	return false;
}

bool FileContent::isInputDirectory()
{
	if (fileStat.st_mode & S_IFDIR)
			return true;
	return false;
}

std::string FileContent::splitFileFromArgs(const std::string &str)
{
	return str.substr(0, str.find("?"));
}

void FileContent::setRange(size_t range)
{
	this->startRange = range;
}

void FileContent::setIsCGI(bool isCgi)
{
	this->isCgi = isCgi;
}