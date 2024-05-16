/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcheel-n <jcheel-n@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 12:46:28 by eavedill          #+#    #+#             */
/*   Updated: 2024/05/15 18:52:13 by jcheel-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "../inc/commonFunctions.hpp"

class Server;
#include "../inc/Server.hpp"

class Client
{
private:
	std::map<std::string, std::string> Request;

public:
	Client();
	Client(std::string const &);
	Client &operator=(Client const &);
	~Client();
	void addKeyReq(std::string const &, std::string const &);
	std::map<std::string, std::string>::iterator findClient(std::string const &);
	std::map<std::string, std::string>::iterator getBeginClient();
	std::map<std::string, std::string>::iterator getEndClient();
	void setHeader(std::string const &);
	std::string getHeader();
	std::string getFileName();
	void clearClient();
	void deleteClient(std::string const &);
	void updateClient(std::string const &, std::string const &);
	void loadCompleteClient(const std::string &);
	std::string getAnswerToSend(Server *server);
	std::string getFilePath(Server *server);
	std::string getFileContent(std::string filename);
	std::string	normalizePath(std::string path);
};

