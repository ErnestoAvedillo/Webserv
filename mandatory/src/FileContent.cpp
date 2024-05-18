#include "../inc/FileContent.hpp"

FileContent::FileContent()
{
	fileName = "";
}
FileContent::FileContent(const std::string &MyfileName) 
{
	this->setFileName(MyfileName);
	openFile();
}

FileContent::~FileContent() {}

int FileContent::openFile()
{
	std::ifstream file;

	std::cout << "Opening file " << fileName << std::endl;
	file.open(fileName.c_str());

	if (file.is_open())
	{
		std::string line;
		while (std::getline(file, line))
		{
			content += line + "\n";
		}
		file.close();
		std::cout << "File open: " << fileName << std::endl;
	}
	else
	{
		std::cout << "Failed to open file: " << fileName << std::endl;
		return 0;
	}
	return 1;
}

std::string FileContent::getContent() 
{
	std::string errorReturn = "Error: " + fileName + " File not found";
	if (!this->openFile())
	{
		std::cout << CHR_RED + errorReturn + RESET << std::endl;
		return ( errorReturn );	
	}
	return content;
}

void FileContent::setFileName(const std::string &file_name)
{
	if (file_name.find("?") != std::string::npos)
		fileName = file_name.substr(0, file_name.find("?"));
	else
		fileName = file_name;
	std::cout << "File name set to: " << fileName << std::endl;
}

std::string FileContent::getFileName()
{
	return fileName;
}
