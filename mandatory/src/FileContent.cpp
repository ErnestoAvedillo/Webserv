#include "../inc/FileContent.hpp"

FileContent::FileContent(Server *srv)
{
	server = srv;
	fileName = "";
	sendComplete = false;
	isFileOpen = false;
	isFistFragment = true;
	isCGI = false;
	CGIFolder = "";
	this->cgiModule = new CGI();
}
FileContent::FileContent(const std::string &MyfileName, Server *srv) 
{
	server = srv;
	isFileOpen = this->setFileName(MyfileName);
	sendComplete = false;
	isFistFragment = true;
	isCGI = false;
	CGIFolder = "";
	this->cgiModule = new CGI();
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
	if (isCGI)
	{
		std::cout << "is a CGI file: " << fileName << std::endl;
		sendComplete = true;
		return cgiModule->execute();
	}
	else
	{
		std::cout << "is not a CGI file: " << fileName << std::endl;
	}
	if (isFileOpen)
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
	else
	{
		content = errorReturn;
	}
	sendComplete = true;
	return content;
}

bool FileContent::setFileName(const std::string &file_name)
{
	std::vector<std::string> tmp;
	bool filefound = false;
	std::cout << "File name: " << file_name << std::endl;
	if (file_name.find("?") != std::string::npos)
	{
		tmp = splitString(file_name, '?');
		fileName = tmp[0];
		std::cout << "File name: " << fileName << std::endl;
		args = splitString(tmp[1], '&');
	}
	else
	{
		fileName = file_name;
	}
	if (stat(fileName.c_str(), &fileStat) == 0)
	{
		filefound = true;
	}
	else
	{
		std::cout << "File <"<< fileName << "> not found: " << filefound << std::endl;

	}
	if(fileName.find(CGIFolder) != std::string::npos)
	{
		if(filefound)
		{
			std::cout << "CGI file found: " << fileName << std::endl;
			isCGI = true;
			std::map <std::string, std::string>::iterator it = this->server->findCGIExtension(this->getFileExtension());
			if(it != this->server->CGIEnd())
			{
				if(it->second.size() != 0)
				{
					cgiModule->setFileName(it->second);
					args.insert(args.begin(), fileName);
				}
			}
			else
			{
				cgiModule->setFileName(fileName);
				cgiModule->setArgs(args);
			}
			isFileOpen = true;
		}
		else
		{
			std::cerr << "CGI file not found: " << fileName << std::endl;
		}
	}
	if(!isCGI)
	{
		if(stat(fileName.c_str(), &fileStat))
		{
			fileName = file_name;
			isFileOpen = this->openFile();
		}
		else
		{
			std::cerr << "File not found en stat: " << fileName << std::endl;

		}
	}
	if (isFileOpen)
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

bool FileContent::isCGIFile()
{
	return isCGI;
}

void FileContent::setCGIFile(bool isCGI)
{
	this->isCGI = isCGI;
}

void FileContent::setCGIFolder(const std::string &folder)
{
	CGIFolder = folder;
}

std::string FileContent::getCGIFolder()
{
	return CGIFolder;
}


std::string FileContent::getFileExtension()
{
	std::string ext = fileName.substr(fileName.find_last_of(".") + 1);
	return ext;
}