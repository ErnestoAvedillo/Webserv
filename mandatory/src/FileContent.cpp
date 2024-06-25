#include "../inc/FileContent.hpp"

// FileContent::FileContent(Server *srv) : StatusCode()
FileContent::FileContent() : StatusCode()
{
	fileName = "";
	startRange = 0;
	sendComplete = false;
	isFileOpen = false;
	isFistFragment = true;
	isAutoIndex = false;
	currentSendingPosition = 0;
	lastSendingPosition = 0;
 	listDir = NULL;
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

std::string FileContent::getContent() 
{
	std::string content;
	if (this->getIsFileOpen())
	{
		 if (isAutoIndex && this->isInputDirectory())
		{
			isFileOpen = true;
			content = listDir->getContentToSend();
			this->setCompleteContentSize(content.size());
			this->setIsSendComplete(listDir->getIsSendComlete());
			return content;
		}
		else if (this->cgiModule->getIsCGI())
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
			this->setIsSendComplete(true);
			return content;
		}
		else
		{
			content = "";
			char buffer[MAX_SENT_BYTES];

			if (this->startRange != 0 && this->getFirstFragment())
			{
				//file.seekg(0, std::ios::beg);
				file.seekg(startRange, std::ios::beg);
				lastSendingPosition = currentSendingPosition;
				file.read(buffer, MAX_SENT_BYTES);
				content.append(buffer, file.gcount());
				//this->setIsSendComplete(true);
			}
			else
			{
				lastSendingPosition = currentSendingPosition;
				file.read(buffer, MAX_SENT_BYTES);
				content.append(buffer, file.gcount());
			}
			currentSendingPosition = file.tellg();
			if (file.eof())
			{
//				std::cout << "Cierro fichero " << fileName << std::endl;
				file.close();
				this->setIsSendComplete(true);
			}
		}
	}
	else
	{
		content += this->getFileContentForStatusCode(this->getCode());
		this->setIsSendComplete(true);
	}
	return (content);
}

bool FileContent::setFileName(const std::string &file_name, const std::string &fileArgs)
{
	bool fileOrFolderExists = this->FileOrFolerExtists(file_name);
	if (fileOrFolderExists)
	{
		if (this->isInputDirectory() && isAutoIndex)
		{

			fileName = file_name;
			this->setIsFileOpen(true);
			listDir = new ListDir(fileName, homeFolder);
			listDir->setSubdirectory(file_name);
			listDir->setContentToList();
		}
		// else if (cgiModule->setIdentifyCGIFromFileName(file_name))
		// else if (this->isCgi)
		else if (cgiModule->getIsCGI() )
		{
			cgiModule->setFileName(file_name, fileArgs);
			this->setIsFileOpen(true);
		}
		else
		{
			// if (this->isInputDirectory())
			// {
			// 	fileName = homeFolder + indexName;
			// }
			// else
			// {
				fileName = file_name;
			// }
			stat(fileName.c_str(), &fileStat);
			completeContentSize = fileStat.st_size;
			this->setIsFileOpen(this->openFile());
			// isFileOpen = true;
			// if (this->getIsFileOpen())
			// 		file.seekg(startRange, std::ios::beg);
		}
	}
	return this->getIsFileOpen();
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

void FileContent::setIsCGI(bool isCgi)
{
	this->cgiModule->setIsCGI(isCgi);
}


void FileContent::setStartRange(long long range)
{
	lastSendingPosition = range;
	this->startRange = range;
}
long long FileContent::getStartRange()
{
	return startRange;
}

void FileContent::setEndRange(long long range)
{
	this->endRange = range;
}


void FileContent::setIsCgi(bool cgi)
{
	this->cgiModule->setIsCGI(cgi);
}

long long FileContent::getFileSize()
{
	return static_cast<long long>(fileStat.st_size);
}

long long FileContent::getCurrentSendingPosition()
{
	if (currentSendingPosition < 0)
		return this->getFileSize()-1;
	return currentSendingPosition;
}

long long FileContent::getLastSendingPosition()
{
	return lastSendingPosition;
}

