
#include "../inc/ListDir.hpp"

ListDir::ListDir() {
	path = "./";
	this->setListOfFiles("./");
	isSendComplete = false;
	files = std::map<std::string, Attributes*>();
}

ListDir::ListDir(const std::string& directory) {
	path = directory.substr(0, directory.find_last_of("/") + 1);
	this->setListOfFiles(directory);
	isSendComplete = false;
	files = std::map<std::string, Attributes*>();
}

ListDir::~ListDir() {
	if (files.empty())
		return;
	std::map <std::string, Attributes *>::iterator itb = files.begin();
	std::map <std::string, Attributes *>::iterator ite = files.end();

	while (itb != ite) 
	{
		std::cout << "Deleting: " << itb->second->getName() << std::endl;
		delete itb->second;
		itb++;
	}
	files.clear();
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
	std::string content;
	content = "<script>\n";
	for (std::map <std::string, Attributes *>::iterator it = files.begin(); it != files.end(); it++) 
	{
		//addRow(".dockerignore",".dockerignore",0,52,"52 B",521717927779,"2024-06-09 12:09:39");
		content = "addRow(\"" + it->second->getName() + "\",";
		content = content + "\"" + it->second->getName() + "\",";
		content = content + (it->second->getIsDir()? "1" : "0") + ",";
		content = content + it->second->getSize() + ",";
		content = content +"\"" + it->second->getSize() + "\",";
		content = content + it->second->getDevice() + ",";
		content = content +"\"" + it->second->getModified() + "\");\n";
	}
	content = content + "</script>\n";
	return content;
}

std::string ListDir::getContentToList() 
{
	std::string content;
	std::string filename = "./www/web5/dir_list.html";
	std::cout << "filename: " << filename << std::endl;
	file.open(filename.c_str(), std::ios::out | std::ios::binary);
	char buffer[MAX_SENT_BYTES];
	if(file.read(buffer, MAX_SENT_BYTES))
	{
		std::cout << "file.read" << std::endl;
		content.append(buffer, file.gcount());
	}
	if(file.eof())
	{
		file.close();
		isSendComplete = true;
	}
	content.append(buffer, file.gcount());
	if(!isSendComplete)
	{
		std::cout << "Send is still nt cmpleted" << std::endl;
		return content;
	}
	content = content  + this->getDirFileList() + "</body></html>";
	return content;
}

bool ListDir::getsIsSendComlete() 
{
	return isSendComplete;
}
