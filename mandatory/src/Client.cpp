/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcheel-n <jcheel-n@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 12:49:08 by eavedill          #+#    #+#             */
/*   Updated: 2024/05/16 02:33:02 by jcheel-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

Client::Client(){}

Client::Client(std::string const &str, Server *srv)
{
	this->server = srv;
	this->loadCompleteClient(str);
	fileContent.setFileName(this->Request[REQ_FILE]);
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

Client::~Client(){}

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
	this->Request[REQ_FILE] = "./" + this->server->getRoot() + value;
	if (value == "/")
		this->Request[REQ_FILE] += this->server->getIndex();
	replaceString(this->Request[REQ_FILE], "%20", " ");
	std::cout << "File: " << this->Request[REQ_FILE] << std::endl;
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
	//std::cout << "Lines: " << lines[0] << std::endl;
	if (parts.size() == 3)
	{
		std::cout << "Parts: " << parts[0] << " " << parts[1] << " " << parts[2] << std::endl;
		this->addKeyType(parts[0]);
		this->addKeyFile(parts[1]);
		this->addKeyVers(parts[2]);
	}
	for (size_t i = 1; i < lines.size(); i++)
			this->addKeyReq(lines[i].substr(0, lines[i].find(":")), lines[i].substr(lines[i].find(":") + 1, lines.size()));
}

std::string Client::getFileContent()
{
	std::string content = this->fileContent.getContent();
	return (content);
}

std::string getExtension(std::string filePath)
{
	size_t point = filePath.find_last_of(".");
	std::string extension = filePath.substr(point + 1, filePath.size());

	std::map<std::string, std::string> Mimetype = create_filetypes();

	if (Mimetype.find(extension) != Mimetype.end())
	{
		std::cout << CHR_BLUE << "found extension " << extension << ": " << Mimetype[extension] << RESET << std::endl;
		return(Mimetype[extension]);
	}
	else
	{
		std::cout << CHR_MGENTA << "NOT found extension " << extension << RESET << std::endl;
		return("text/html"); 
	}
}

std::string Client::getAnswerToSend()
{	
	std::string answer;
	std::string filePath = this->fileContent.getFileName();
	std::string file_content = getFileContent();
	std::cout << "File path: " << filePath << std::endl;
	//std::cout << "File content: " << file_content << std::endl;
	if (this->fileContent.getFirstFragment())
	{
		answer += "HTTP/1.1 200 OK\r\nContent-Type: " + getExtension(filePath) + "\r\n" + "Content - Length : " + toString(file_content.size()) + "\r\n\r\n" + file_content;
		this->fileContent.setFirstFragment(false);
	}
	else
		answer += file_content;

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

std::string Client::getFilePath(Server *server)
{

	std::cout << "Normalizing Path: " << normalizePath(server->getRoot()) << std::endl;
	std::string filePath = normalizePath(server->getRoot()) +  this->Request[REQ_FILE];
	if (filePath.at(filePath.size() - 1) == '/')
		filePath += server->getIndex();
	filePath = filePath.substr(0, filePath.find("?"));
	std::cout << "MY File Path: " << filePath << std::endl;

	return (filePath);
}

std::string Client::getFileContent(std::string filename)
{
	std::ifstream file;
	std::string file_content;
	file.open(filename);
	if (!file)
	{
		std::cerr << "File not found" << std::endl;
		errno = 0;
		return ("HTTP/1.1 404 Not Found\r\n\r\n");
	}
	std::string line;
	while (std::getline(file, line))
		file_content += line + "\n";
	file.close();
	return (file_content);

}

std::string getExtension(std::string filePath)
{
	size_t point = filePath.find_last_of(".");
	std::string extension = filePath.substr(point + 1, filePath.size());

	/* Create once only */
	std::map<std::string, std::string> Mimetype = create_filetypes();

	std::cout << "found extension " << extension << std::endl;
	if (Mimetype.find(extension) != Mimetype.end())
		return(Mimetype[extension]);
	else
		return("text/html"); 
}

std::string Client::getAnswerToSend(Server *server)
{	
	std::string answer;
	std::string filePath = getFilePath(server);
	std::string file_content = getFileContent(filePath);
	if (file_content.find("HTTP/1.1 404 Not Found\r\n\r\n") != std::string::npos)
		return ("HTTP/1.1 404 Not Found\r\n\r\n");
	answer += "HTTP/1.1 200 OK\r\nContent-Type: " + getExtension(filePath) + "\r\nContent-Lenght: " + std::to_string(file_content.size())  + "\r\n\r\n";
	std::cout << getExtension(filePath) << std::endl;
	
	std::cout << "File Path: " << filePath << "$" << std::endl;
	// std::ifstream file;
	std::cout << "File Path: " << filePath << std::endl;
	answer += file_content;
	return (answer);
}

bool Client::isSendComplete()
{
	return this->fileContent.isSendComplete();
}
