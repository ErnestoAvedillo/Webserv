/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 12:49:08 by eavedill          #+#    #+#             */
/*   Updated: 2024/05/25 15:52:20 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

Client::Client(){}

Client::Client(Server *srv)
{
	this->server = srv;
}

Client::Client(std::string const &str, Server *srv)
{
	this->server = srv;
	this->loadCompleteClient(str);
}

Client &Client::operator=(Client const &rsh)
{
	std::map<std::string, std::string>::const_iterator itb = rsh.Request.begin();
	std::map<std::string, std::string>::const_iterator ite = rsh.Request.end();
	while(itb != ite)
	{
		this->Request[itb->first] = itb->second;
		itb++;
	}
	return (*this);
}

Client::~Client()
{
}

void Client::addKeyReq(std::string const &key, std::string const &value)
{
	this->Request[key] = value;
}

void Client::addKeyType(std::string const &value)
{
	this->Request[REQ_TYPE] = value;
}

void Client::addKeyFile(std::string const &value)
{
	this->Request[REQ_FILE] = this->server->getRoot() + value;
	if (value == "/")
		this->Request[REQ_FILE] += this->server->getIndex();
	replaceString(this->Request[REQ_FILE], "%20", " ");
}

void Client::addKeyVers(std::string const &value)
{
	this->Request[REQ_VER] = value;
}

std::map<std::string, std::string>::iterator Client::findClient(std::string const &key)
{
	return (this->Request.find(key));
}

std::map<std::string, std::string>::iterator Client::getBeginClient()
{
	return (this->Request.begin());
}

std::map<std::string, std::string>::iterator Client::getEndClient()
{
	return (this->Request.end());
}

void Client::clearClient()
{
	this->Request.clear();
}

void Client::deleteClient(std::string const &key)
{
	this->Request.erase(key);
}

void Client::updateClient(std::string const &key, std::string const &value)
{
	this->Request[key] = value;
}

void Client::loadCompleteClient( std::string const &str)
{
	std::vector<std::string> lines = splitString(str, '\n');
	std::vector<std::string> parts = splitString(lines[0], ' ');
	if (parts.size() == 3)
	{
		this->addKeyType(parts[0]);
		this->addKeyFile(parts[1]);
		this->addKeyVers(parts[2]);
	}
	for (size_t i = 1; i < lines.size(); i++)
			this->addKeyReq(lines[i].substr(0, lines[i].find(":")), lines[i].substr(lines[i].find(":") + 1, lines.size()));
	this->loadDataHeader();
}


void Client::getExtension()
{
	size_t point = this->Request[REQ_FILE].find_last_of(".");
	std::string extension = this->Request[REQ_FILE].substr(point + 1, this->Request[REQ_FILE].size());

	/* Create once only */
	std::map<std::string, std::string> Mimetype = create_filetypes();
	if (Mimetype.find(extension) != Mimetype.end())
		header.setContentType(Mimetype[extension]);
	else
		header.setContentType("text/html");

}

/*
Normalize the path, removes .., adds ./ at teh beggining if necessary, removes / at the end, removes duplicate /.
*/
std::string	Client::normalizePath(std::string path)
{
	while (path.find("..") != std::string::npos)
		path.erase(path.find(".."), 2);

	std::vector<std::string> parts = splitString(path, '/');
	std::string normalizePath;
	for (size_t i = 0; i < parts.size(); i++)
	{
		if (i == 0)
		{
			if (parts[i] == ".")
				normalizePath = parts[i] + "/";
			else if (i == (parts.size() - 1))
				normalizePath = "/" + parts[i];
			else
				normalizePath = "/" + parts[i] + "/";
		}
		else if (i == (parts.size() - 1))
			normalizePath += parts[i];
		else
			normalizePath += parts[i] + "/";
	}
	return (normalizePath);
}

std::string Client::getFilePath()
{
	std::string filePath = normalizePath(server->getRoot()) +  this->Request[REQ_FILE];
	if (filePath.at(filePath.size() - 1) == '/')
		filePath += server->getIndex();
	filePath = filePath.substr(0, filePath.find("?"));
	return (filePath);
}

std::string Client::getFileContent()
{
	std::string content = this->fileContent.getContent();
	return (content);
}

std::string Client::getAnswerToSend()
{	
	std::string answer;
	std::string filePath = this->fileContent.getFileName();
	std::string file_content = getFileContent();
	if (this->fileContent.getFirstFragment())
	{
		answer += header.generateHeader() + file_content;
		this->fileContent.setFirstFragment(false);
	}
	else
		answer += file_content;
	return (answer);
}

bool Client::isSendComplete()
{
	return this->fileContent.isSendComplete();
}

void Client::loadDataHeader()
{
	if (fileContent.setFileName(this->Request[REQ_FILE]))
	{
		header.setLastModified(fileContent.getLastModified());
		this->getExtension();
		header.setContentLength(fileContent.getContentSize());
		header.setStatus("200 OK");
		header.setServer(server->getServerName());
	}
	else
		header.setStatus("404 Not Found");
}
