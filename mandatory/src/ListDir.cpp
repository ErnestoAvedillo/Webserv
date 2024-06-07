
#include "../inc/ListDir.hpp"

ListDir::ListDir() {
}

ListDir::ListDir(const std::string& directory) {
	path = directory;
	this->setListOfFiles(directory);
}

ListDir::~ListDir() {
}

void ListDir::setListOfFiles(const std::string& directory) 
{
	path = directory;
	DIR* dir = opendir(directory.c_str());
	if (dir) {
		struct dirent* entry;
		while ((entry = readdir(dir)) != nullptr) 
		{
			Attributes *attribute = new Attributes(entry->d_name);
			files.insert(std::pair <std::string, Attributes *>{attribute->getName(), attribute});
		}
		closedir(dir);
	}
}

std::map <std::string, Attributes *> ListDir::getBeginOfFiles() 
{
	return files;
}

std::map <std::string, Attributes *> ListDir::getEndOfFiles() 
{
	return files;
}



