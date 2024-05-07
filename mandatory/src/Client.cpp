/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 12:49:08 by eavedill          #+#    #+#             */
/*   Updated: 2024/05/05 15:09:08 by eavedill         ###   ########.fr       */
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

std::string Client::getFilePath(Server *server)
{
	(void)server;
	
	
	
	std::string filePath = "./"  + server->getRoot() +  this->Request[REQ_FILE];


	return (filePath);

	// std::string filePath = this->Request[REQ_FILE];
	// if (filePath.find("?") != std::string::npos)
	// {
	// 	filePath = filePath.substr(0, filePath.find("?"));
	// 	// this->Request[REQ_FILE] = filePath;
	// }
	
	// return (filePath);

}

std::string Client::getFileContent(std::string filename)
{
	std::ifstream file;
	std::string file_content;
	file.open(filename, std::ios::in);
	if (!file)
	{
		std::cerr << "File not found" << std::endl;
		return ("HTTP/1.1 404 Not Found\r\n\r\n");
	}
	std::string line;
	while (std::getline(file, line))
		file_content += line;
	file.close();
	return (file_content);

}

std::string Client::getAnswerToSend(Server *server)
{
	
	(void)server;
	//std::string answer = this->Request[REQ_VER] + " 200 OK\n";
	// std::string answer = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello, World!</h1></body></html>\r\n";

	
	std::string answer;
	std::string filePath = getFilePath(server);
	std::string file_content = getFileContent(filePath);
	// Get File Content
	// std::string filePath = this->Request[REQ_FILE];
	// std::cout << 
	if (filePath.find(".jpg") != std::string::npos)
		answer = "HTTP/1.1 200 OK\r\nContent-Type: image/jpg\r\n\r\n";
	else	
		answer = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
	
	std::cout << "File Path: " << filePath << "$" << std::endl;
	std::ifstream file;
	std::string file_content;
	// filePath = "." + filePath;
	std::cout << "File Path: " << filePath << std::endl;
	file.open(filePath, std::ios::in);
	if (!file)
	{
		std::cerr << "File not found" << std::endl;
		return ("HTTP/1.1 404 Not Found\r\n\r\n");
	}
	std::string line;
	while (std::getline(file, line))
		file_content += line;
	file.close();

	answer += file_content;
	//std::cout << "answer: " << answer << std::endl;
	// std::cout << "File Content: " << file_content << std::endl;
	// std::cout << "Answer: " << answer << std::endl;
	// answer += file_content.size()
	return (answer);
}
