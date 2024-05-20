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
	this->setFileName(MyfileName);
	sendComplete = false;
	isFileOpen = false;
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
		while (file.read(buffer, MAX_SENT_BYTES))
		{
			content.append(buffer, file.gcount());
		}
		content.append(buffer, file.gcount());
//		std::string line;
//		content = "";
//		while (std::getline(file, line))
//		{
//			content += line; //+ "\n";
							 // if (content.size() >= MAX_SENT_BYTES)
			// {
			// 	return content;
			// }
//		}
		file.close();
		std::cout << "File closed: " << fileName << std::endl;
		//std::cout << "File content: " << content << std::endl;
	}
	else
	{
		//std::cout << CHR_RED + errorReturn + RESET << std::endl;
		content = errorReturn;
	}
	sendComplete = true;
	//std::cout << "File content: " << content << std::endl;
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
