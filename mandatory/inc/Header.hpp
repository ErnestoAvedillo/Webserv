/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcheel-n <jcheel-n@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 16:05:31 by eavedill          #+#    #+#             */
/*   Updated: 2024/06/14 23:38:42 by jcheel-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <map>
#include <iostream>
#include <ctime>
#include "../inc/utils.hpp"
#include <algorithm>
class Header 
{
	private:
		/* Receive Header*/
		std::string method;
		std::string path;
		
		std::string protocol;
		std::string status;
		std::string server;
		std::string date;
		std::string lastModified;
		size_t contentLength;
		std::string contentType;
		std::map<std::string, std::string> attributes;
		/*
		HTTP/1.0 200 OK
		Server: Netscape-Communications/1.1
		Date: Tuesday, 25-Nov-97 01:22:04 GMT
		Last-modified: Thursday, 20-Nov-97 10:44:53 GMT
		Content-length: 6372
		Content-type: text/html
		*/
public:
	Header();
	Header(std::string header);
	~Header();
	std::string generateHeader() const;
	std::map<std::string, std::string> getAttributes();
	void setProtocol(std::string protocol);
	void setServer(std::string server);
	void setStatus(std::string status);
	void setDate();
	void setLastModified(std::string lastModified);
	void setContentLength(size_t contentLength);
	void setContentType(std::string contentType);
	void setAttribute(std::string key, std::string value);
	std::string getMethod();
	std::string getPath();
};
