/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcheel-n <jcheel-n@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 12:46:28 by eavedill          #+#    #+#             */
/*   Updated: 2024/05/20 13:49:29 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "../inc/utils.hpp"

class Server;
#include "../inc/Server.hpp"

#include "../inc/FileContent.hpp"
#include "../inc/toString.hpp"
#include "../inc/colors.h"

#define REQ_TYPE "Type"
#define REQ_FILE "File"
#define REQ_VER "Version"
#define REQ_CONTENT "Content-Type"

class Server;
#include "../inc/Server.hpp"

class Client
{
private:
	std::map<std::string, std::string> Request;
	FileContent fileContent;
	Server *server;
public:
	Client();
	Client(std::string const &, Server *);
	Client &operator=(Client const &);
	~Client();
	void addKeyReq(std::string const &, std::string const &);
	void addKeyType(std::string const &);
	void addKeyFile(std::string const &);
	void addKeyVers(std::string const &);
	std::map<std::string, std::string>::iterator findClient(std::string const &);
	std::map<std::string, std::string>::iterator getBeginClient();
	std::map<std::string, std::string>::iterator getEndClient();
	void clearClient();
	void deleteClient(std::string const &);
	void updateClient(std::string const &, std::string const &);
	void loadCompleteClient(const std::string &);
	std::string getAnswerToSend(Server *server);
	std::string getFilePath(Server *server);
	std::string getFileContent(std::string filename);
	std::string	normalizePath(std::string path);
	std::string getFileContent();
	bool isSendComplete();
};

