#include "../inc/FileContent.hpp"

FileContent::FileContent(Server *srv)
{
	server = srv;
	fileName = "";
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
			sendComplete = true;
			content = cgiModule->execute();
			contentSize = content.size();
			return content;
		}
		else if (server->getAutoIndex() && this->fileStat.st_mode & S_IFDIR)
		{
			content = listDir->getContentToSend();
			sendComplete = listDir->getIsSendComlete();
			contentSize = content.size();
			return content;
		}
		else
		{
			content = "";
			char buffer[MAX_SENT_BYTES];
			contentSize = fileStat.st_size;
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
		if (this->isInputDirectory() && server->getAutoIndex())
		{
			fileName = FileAndFolder;
			isFileOpen = true;
			listDir->setContentToList();
			return isFileOpen;
		}
		else if (cgiModule->setIsCGI(file_name))

	std::string tmp = file_name.substr(0, file_name.find("?"));
	// bool filefound = false;
	// std::cout << "File name: " << file_name << std::endl;
	if (stat(tmp.c_str(), &fileStat) == 0)
	{
		// filefound = true;
		file_name = tmp;
	}
	else
	{
		// std::cout << "File <" << file_name << "> not found: " << filefound << std::endl;
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
			cgiModule->setFileName(file_name);
			isFileOpen = true;
			return isFileOpen;
		}
		else
		{
			if (this->isInputDirectory())
				fileName = FileAndFolder + server->getIndex();
			else
				fileName = FileAndFolder;
			isFileOpen = this->openFile();
			isFileOpen = true;
			return isFileOpen;
		}
	}
  else
	{
		std::cout << "File <" << file_name << "> not found." << std::endl;
		//throw std::runtime_error("Error: File not found");
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
	return contentSize;
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