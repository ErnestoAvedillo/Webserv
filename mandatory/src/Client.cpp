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

#define REQ_TYPE "Type"
#define REQ_FILE "File"
#define REQ_VER "Version"
#define REQ_CONTENT "Content-Type"


Client::Client(){}

Client::Client(std::string const &str)
{
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

Client::~Client(){}

void Client::addKeyReq(std::string const &key, std::string const &value)
{
	this->Request[key] = value;
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
		this->addKeyReq(REQ_TYPE, parts[0]);
		this->addKeyReq(REQ_FILE, parts[1]);
		this->addKeyReq(REQ_VER, parts[2]);
	}
	for (size_t i = 1; i < lines.size(); i++)
			this->addKeyReq(lines[i].substr(0, lines[i].find(":")), lines[i].substr(lines[i].find(":") + 1, lines.size()));
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
	file.open(filename, std::ios::binary);
	if (!file)
	{
		std::cerr << "File not found" << std::endl;
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
	// Get File Content
	// std::string filePath = this->Request[REQ_FILE];
	// std::cout << 
	if (file_content.empty())
		return ("HTTP/1.1 404 Not Found\r\n\r\n");
	answer += "HTTP/1.1 200 OK\r\nContent-Type: " + getExtension(filePath) + "\r\nContent-Lenght: " + std::to_string(file_content.size())  + "\r\n\r\n";
	std::cout << getExtension(filePath) << std::endl;
	
	std::cout << "File Path: " << filePath << "$" << std::endl;
	std::ifstream file;
	std::cout << "File Path: " << filePath << std::endl;
	answer += file_content;
	return (answer);
}
