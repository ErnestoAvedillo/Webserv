
#include "../inc/ListDir.hpp"

ListDir::ListDir() {
	path = "./";
	this->setListOfFiles();
	isSendComplete = false;
	this->openMasterListFile();
}

ListDir::ListDir(const std::string& directory) {
//	std::cout << "ListDir::ListDir(const std::string& directory)" << std::endl;
//	path = directory.substr(0, directory.find_last_of("/") + 1);
	if (directory[directory.size() - 1] != '/')
		path = directory + "/";
	else
		path = directory;
	this->setListOfFiles();
	isSendComplete = false;
	this->openMasterListFile();
}

ListDir::~ListDir() {
	std::map <std::string, Attributes *>::iterator itb = files.begin();
	std::map <std::string, Attributes *>::iterator ite = files.end();

	while (itb != ite) 
	{
		std::cout << "Deleting: " << itb->second->getName() << std::endl;
		delete itb->second;
		itb++;
	}
	files.clear();
	file.close();
}

void ListDir::setListOfFiles() 
{
	std::cout <<RED<< "ListDir::setListOfFiles()" <<RESET<< std::endl;
	DIR* dir = opendir(path.c_str());
	if (dir) {
		struct dirent* entry;
		while ((entry = readdir(dir)) != nullptr) 
		{
			Attributes *attribute = new Attributes(path + entry->d_name);
			files.insert(std::pair <std::string, Attributes *>{attribute->getName(), attribute});
		}
		closedir(dir);
	}
}

std::map <std::string, Attributes *>::iterator ListDir::getBeginOfFiles() 
{
	return files.begin();
}

std::map <std::string, Attributes *>::iterator ListDir::getEndOfFiles() 
{
	return files.end();
}

std::string ListDir::getDirFileList() 
{
	std::string content = "";
	for (std::map <std::string, Attributes *>::iterator it = files.begin(); it != files.end(); it++) 
	{
		content = content + "<script>\n";
		//addRow(".dockerignore",".dockerignore",0,52,"52 B",521717927779,"2024-06-09 12:09:39");
		content = content + "addRow(\"" + it->second->getName() + "\",";
		content = content + "\"" + it->second->getName() + "\",";
		content = content + (it->second->getIsDir()? "1" : "0") + ",";
		content = content + it->second->getSize() + ",";
		content = content +"\"" + it->second->getSize() + "\",";
		content = content + it->second->getDevice() + ",";
		content = content +"\"" + it->second->getModified() + "\");\n";
		content = content + "</script>\n";
	}
	return content;
}

std::string ListDir::getContentToList() 
{
	std::string content;

	char buffer[MAX_SENT_BYTES];
	if(file.read(buffer, MAX_SENT_BYTES))
	{
		std::cout << "file.read" << std::endl;
		content.append(buffer, file.gcount());
	}
	if(file.eof())
	{
		isSendComplete = true;
	}
	content.append(buffer, file.gcount());
	if(!isSendComplete)
	{
		std::cout << "Send is still not cmpleted" << std::endl;
		return content;
	}

	content = content + this->getDirFileList() + "</body></html>";
	std::cout << "Send is completed" << content << std::endl;
	return content;
}

bool ListDir::getsIsSendComlete() 
{
	return isSendComplete;
}

void ListDir::openMasterListFile() 
{
	std::string filename = path + "dir_list.html";
	std::cout << "filename: " << filename << std::endl;
	file.open(filename.c_str(), std::ios::out | std::ios::binary);
}

void ListDir::printFiles() 
{
	std::map <std::string, Attributes *>::iterator itb = files.begin();
	std::map <std::string, Attributes *>::iterator ite = files.end();
	
	std::cout << YELLOW << "---printing list of files--- " << RESET << std::endl;
	while (itb != ite) 
	{
		std::cout << "Name: " << itb->second->getName() << std::endl;
		std::cout << "Size: " << itb->second->getSize() << std::endl;
		std::cout << "Device: " << itb->second->getDevice() << std::endl;
		std::cout << "Modified: " << itb->second->getModified() << std::endl;
		std::cout << "IsDir: " << itb->second->getIsDir() << std::endl;
		itb++;
	}
	std::cout << CYAN << "---end of list of files--- " << RESET << std::endl;
}
