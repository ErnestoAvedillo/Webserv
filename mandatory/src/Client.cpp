/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 12:49:08 by eavedill          #+#    #+#             */
/*   Updated: 2024/05/04 13:18:49 by eavedill         ###   ########.fr       */
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
	if (lines.size() == 3)
	{
		this->addKeyReq(REQ_TYPE, parts[0]);
		this->addKeyReq(REQ_FILE, parts[1]);
		this->addKeyReq(REQ_VER, parts[2]);
	}
	for (size_t i = 1; i < lines.size(); i++)
	{
		std::vector<std::string> parts = splitString(lines[i], ':');
		if (parts.size() == 2)
			this->addKeyReq(parts[0], parts[1]);
	}
}
std::string Client::getAnswerToSend()
{
	std::string answer = this->Request[REQ_VER] + " 200 OK\n";
	
	std::map<std::string, std::string>::iterator itb = this->Request.begin();
	std::map<std::string, std::string>::iterator ite = this->Request.end();
	while(itb != ite)
	{
		if (itb->first != REQ_VER)
			answer += itb->first + ": " + itb->second + "\n";
		itb++;
	}
	return (answer);
}
