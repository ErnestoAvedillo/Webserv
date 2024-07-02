/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListDir.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42barcelona>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 13:51:56 by eavedill          #+#    #+#             */
/*   Updated: 2024/07/02 23:14:13 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../inc/ListDir.hpp"

ListDir::ListDir() 
{
	posToSend = 0;
	path = "./";
	isSendComplete = false;
	this->openMasterListFile();
}

ListDir::ListDir(const std::string& directory, const std::string& homeFolder) 
{
	homePath = homeFolder + "/";
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
		while ((entry = readdir(dir)) != NULL) 
		{
			Attributes *attribute = new Attributes(path + entry->d_name);
			files.insert(std::pair<std::string, Attributes *>(entry->d_name, attribute));
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
	std::map <std::string, Attributes *>::iterator itb = files.begin();
	itb++;
	itb++;
	std::map<std::string, Attributes *>::iterator ite = files.end();
	while (itb != ite)
	{
		content = content + "<script>\n";
		content = content + "addRow(\" " + itb->first + "\",";
		content = content + "\"" + itb->first + "\",";
		content = content + (itb->second->getIsDir()? "1" : "0") + ",";
		content = content + itb->second->getSize() + ",";
		content = content +"\"" + itb->second->getSize() + "\",";
		content = content + itb->second->getDevice() + ",";
		content = content +"\"" + itb->second->getModified() + "\");\n";
		content = content + "</script>\n";
		itb++;
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
	if (homePath != path)
	{
		contentToSend += ("<script>start(\"" + path + "\");</script>\n");
		contentToSend += "<script>onHasParentDirectory();</script>\n";
	}
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
	std::string filename = TEMPLATE_LIST_DIR;
	file.open(filename.c_str(), std::ios::out | std::ios::binary);
	if (!file.is_open())
	{
		isFileOpen = false;
		return;
	}
	isFileOpen = true;
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
	if (subDir == "/")
		this->path += subDir.substr(1);
	else
	{
		this->path = subDir;
	}
	path += (path[path.size() -1] == '/') ? "" :"/";
	this->setListOfFiles();
}

bool ListDir::getIsFileOpen()
{
	return isFileOpen;
}