#include "../inc/FileContent.hpp"

// FileContent::FileContent(Server *srv) : StatusCode()
FileContent::FileContent() : StatusCode()
{
	//server = srv;
	// this->loadErrorPageFromDir(srv->getErrorPage());
	fileName = "";
	startRange = 0;
	sendComplete = false;
	isFileOpen = false;
	isFistFragment = true;
	//isAutoIndex = false;
	// isAutoIndex = srv->getAutoIndex();
	// indexInHomeFolder = srv->getRoot() + srv->getIndex();
	// this->cgiModule = srv->cgiModuleClone();
	// if (srv->getAutoIndex())
	// {
	// listDir = new ListDir("./");
	// 	isFileOpen = true;
	// }
	// else
	// {
	 	listDir = NULL;
	// }
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
	{
		return 1;
	}
	return 0;
}

std::string FileContent::getContent(size_t startRange) 
{
	std::string content;
	if (this->getIsFileOpen())
	{
		if (cgiModule->getIsCGI())
		{
			this->setIsSendComplete(true);
			try
			{
				content = cgiModule->execute();
				this->setCompleteContentSize(content.size());
			}
			catch (int e)
			{
				content = this->getFileContentForStatusCode(e);
			}
			return content;
		}
		else if (isAutoIndex && this->isInputDirectory())
		{
			isFileOpen = true;
			content = listDir->getContentToSend();
			this->setCompleteContentSize(content.size());
			this->setIsSendComplete(listDir->getIsSendComlete());
			return content;
		}
		else
		{
			content = "";
			char buffer[MAX_SENT_BYTES];
			(void)startRange;
			if (startRange)
				file.seekg(startRange, std::ios::beg);
			// std::cout << "enviando paquete:" << file.tellg() << std::endl;
			if (file.read(buffer, MAX_SENT_BYTES))
			{
				if (file.eof())
				{
					file.close();
					this->setIsSendComplete(true);
				}
				content.append(buffer, file.gcount());
				return content;
			}
			else
			{
				std::cout << "Cierro fichero " <<fileName<< std::endl;
				file.close();
				content.append(buffer, file.gcount());
			}
		}
	}
	else
	{
		content = this->getCodeContent(NOT_FOUND_CODE);
	}
	this->setIsSendComplete(true);
	return (content);
}

bool FileContent::setFileName(const std::string &file_name)
{
	std::string FileAndFolder = this->splitFileFromArgs(file_name);
	bool fileOrFolderExists = this->FileOrFolerExtists(FileAndFolder);
	if (fileOrFolderExists)
	{
		if (this->isInputDirectory() && isAutoIndex)
		{

			fileName = FileAndFolder;
			this->setIsFileOpen(true);
			listDir = new ListDir(fileName);
			listDir->setSubdirectory(FileAndFolder);
			listDir->setContentToList();
			return this->getIsFileOpen();
		}
		//else if (cgiModule->setIsCGI(file_name))
		else if (this->isCgi)
		{
			cgiModule->setFileName(file_name);
			this->setIsFileOpen(true);
			return this->getIsFileOpen();
		}
		else
		{
			if (this->isInputDirectory())
			{
				fileName = homeFolder + indexName;
			}
			else
			{
				fileName = FileAndFolder;
			}
			stat(fileName.c_str(), &fileStat);
			completeContentSize = fileStat.st_size;
			this->setIsFileOpen(this->openFile());
			// isFileOpen = true;
			// if (this->getIsFileOpen())
			// 		file.seekg(startRange, std::ios::beg);
			return this->getIsFileOpen();
		}
	}
	return this->getIsFileOpen();
}
void FileContent::setIsAutoIndex(bool autoIndex)
{
	this->isAutoIndex = autoIndex;
}

std::string FileContent::getFileName()
{
	return fileName;
}

void FileContent::setIsFileOpen(bool isFileOpen)
{
	this->isFileOpen = isFileOpen;
}

bool FileContent::getIsFileOpen()
{
	return isFileOpen;
}

void FileContent::setIsSendComplete(bool value){
	this->sendComplete = value;
}

bool FileContent::getIsSendComplete()
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

void FileContent::setCompleteContentSize(size_t size)
{
	completeContentSize = size;
}

size_t FileContent::getCompleteContentSize()
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

void FileContent::setIndexName(const std::string &index)
{
	indexName = index;
}

std::string FileContent::getIndexName()
{
	return indexName;
}

void FileContent::setHomeFolder(const std::string &home)
{
	homeFolder = home;
}

std::string FileContent::getHomeFolder()
{
	return homeFolder;
}

void FileContent::setCGIModule(CGI *cgi)
{
	cgiModule = cgi;
}

void FileContent::setIsAutoIndex(bool autoIndex)
{
	isAutoIndex = autoIndex;
}

bool FileContent::getIsAutoIndex()
{
	return isAutoIndex;
}
