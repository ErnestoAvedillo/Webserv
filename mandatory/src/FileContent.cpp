#include "../inc/FileContent.hpp"

FileContent::FileContent()
{
	fileName = "";
}
FileContent::FileContent(const std::string &MyfileName) : fileName(MyfileName)
{
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
	if (!this->openFile())
		return ("Error: " + fileName + " File not found");	
	return content;
}

void FileContent::setFileName(const std::string &file_name)
{
	this->fileName = file_name;
}

std::string FileContent::getFileName()
{
	return fileName;
}