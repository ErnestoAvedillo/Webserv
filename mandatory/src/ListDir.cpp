
#include "../inc/ListDir.hpp"

ListDir::ListDir() 
{
	posToSend = 0;
	path = "./";
	isSendComplete = false;
	this->openMasterListFile();
}

ListDir::ListDir(const std::string& directory) 
{
	posToSend = 0;
	if (directory[directory.size() - 1] != '/')
		path = directory + "/";
	else
		path = directory;
	isSendComplete = false;
	this->openMasterListFile();
}

ListDir::~ListDir() {
	std::map <std::string, Attributes *>::iterator itb = files.begin();
	std::map <std::string, Attributes *>::iterator ite = files.end();

	while (itb != ite) 
	{
		delete itb->second;
		itb++;
	}
	files.clear();
	file.close();
}

void ListDir::setListOfFiles() 
{
	DIR* dir = opendir(path.c_str());
	if (dir) {
		struct dirent* entry;
		while ((entry = readdir(dir)) != nullptr) 
		{
			Attributes *attribute = new Attributes(path + entry->d_name);
			files.insert(std::pair<std::string, Attributes *>{entry->d_name, attribute});
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
	for (std::map <std::string , Attributes *>::iterator it = files.begin(); it != files.end(); it++) 
	{
		content = content + "<script>\n";
		//addRow(".dockerignore",".dockerignore",0,52,"52 B",521717927779,"2024-06-09 12:09:39");
		content = content + "addRow(\" " + it->first + "\",";
		content = content + "\"" + it->first + "\",";
		content = content + (it->second->getIsDir()? "1" : "0") + ",";
		content = content + it->second->getSize() + ",";
		content = content +"\"" + it->second->getSize() + "\",";
		content = content + it->second->getDevice() + ",";
		content = content +"\"" + it->second->getModified() + "\");\n";
		content = content + "</script>\n";
	}
	return content;
}

void ListDir::setContentToList() 
{
	contentToSend = "";

	char buffer[MAX_SENT_BYTES];
	while(file.read(buffer, MAX_SENT_BYTES))
	{
		contentToSend.append(buffer, file.gcount());
	}
	contentToSend.append(buffer, file.gcount());
	//replaceString(contentToSend, "LOCATION", path);
	contentToSend = contentToSend + "<body>" + this->getDirFileList() + "</body></html>";
}

void ListDir::setIsSendComlete() 
{
	if(posToSend >= contentToSend.size())
		isSendComplete = true;
}

bool ListDir::getIsSendComlete()
{
		return isSendComplete;
}

void ListDir::openMasterListFile() 
{
	std::string filename = "./Master/dir_list.html";
	file.open(filename.c_str(), std::ios::out | std::ios::binary);
}

std::string ListDir::getContentToSend() 
{
	std::string subStrToSend = contentToSend.substr(posToSend, MAX_SENT_BYTES);
	if(posToSend + MAX_SENT_BYTES >= contentToSend.size())
		posToSend = contentToSend.size();
	else
		posToSend += MAX_SENT_BYTES;
	this->setIsSendComlete();
	return subStrToSend;
}

size_t ListDir::getSizeToSend()
{
	return contentToSend.size();
}

void ListDir::setSubdirectory(const std::string &subDir)
{
	if (subDir[0] == '/')
		path += subDir.substr(1);
	else
		path = subDir;
	path += (path[path.size() -1] == '/') ? "" :"/";
	this->setListOfFiles();
}