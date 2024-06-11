/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcheel-n <jcheel-n@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 16:05:31 by eavedill          #+#    #+#             */
/*   Updated: 2024/06/11 14:44:49 by jcheel-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <map>
#include <iostream>
#include <ctime>
#include "../inc/utils.hpp"

class Header 
{
	private:
		std::string Version;
		std::string Status;
		std::string Server;
		std::string Date;
		std::string LastModified;
		size_t ContentLength;
		std::string ContentType;
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
	~Header();
	std::string generateHeader() const;
	void setVersion(std::string version);
	void setStatus(std::string status);
	void setServer(std::string server);
	void setDate();
	void setLastModified(std::string lastModified);
	void setContentLength(size_t contentLength);
	void setContentType(std::string contentType);
};
