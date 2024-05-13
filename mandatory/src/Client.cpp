/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcheel-n <jcheel-n@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 12:49:08 by eavedill          #+#    #+#             */
/*   Updated: 2024/05/10 23:21:42 by jcheel-n         ###   ########.fr       */
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
	filePath = filePath.substr(0, filePath.find("?"));
	std::cout << "MY File Path: " << filePath << std::endl;

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

std::string getExtension(std::string filePath)
{
	size_t point = filePath.find_last_of(".");
	std::string extension = filePath.substr(point + 1, filePath.size());

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
	if (file_content.find("HTTP/1.1 404"))

	answer += "HTTP/1.1 200 OK\r\nContent-Type: " + getExtension(filePath) + "\r\nContent-Lenght: " + std::to_string(file_content.size())  + "\r\n\r\n";
	std::cout << getExtension(filePath) << std::endl;
	

	// if (filePath.find(".jpg") != std::string::npos)
	// 	answer = "HTTP/1.1 200 OK\r\nContent-Type: image/jpg\r\n\r\n";
	// else if (filePath.find(".js") != std::string::npos)
	// 	answer = "HTTP/1.1 200 OK\r\nContent-Type: application/javascript\r\n\r\n";
	// else if (filePath.find(".css") != std::string::npos)
	// 	answer = "HTTP/1.1 200 OK\r\nContent-Type: text/css\r\n\r\n";
	// else	
	// 	answer = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
	
	std::cout << "File Path: " << filePath << "$" << std::endl;
	std::ifstream file;
	// std::string file_content;
	// filePath = "." + filePath;
	std::cout << "File Path: " << filePath << std::endl;
	// file.open(filePath, std::ios::in);
	// if (!file)
	// {
	// 	std::cerr << "File not found" << std::endl;
	// 	return ("HTTP/1.1 404 Not Found\r\n\r\n");
	// }
	// std::string line;
	// while (std::getline(file, line))
	// 	file_content += line;
	// file.close();

	answer += file_content;
	//std::cout << "answer: " << answer << std::endl;
	// std::cout << "File Content: " << file_content << std::endl;
	// std::cout << "Answer: " << answer << std::endl;
	// answer += file_content.size()
	return (answer);
}
