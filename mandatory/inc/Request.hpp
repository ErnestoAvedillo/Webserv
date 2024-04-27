/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 17:42:05 by eavedill          #+#    #+#             */
/*   Updated: 2024/04/27 10:39:48 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include "../inc/Header.hpp"
class Request
{
private:
	Header header;
	std::map<std::string, std::string> request;

public:
	Request();
	Request(std::string const &);
	Request &operator=(Request const &);
	~Request();
	void addRequest(std::string const &, std::string const &);
	std::map<std::string, std::string>::iterator findRequest(std::string const &);
	std::map<std::string, std::string>::iterator getBeginRequest();
	std::map<std::string, std::string>::iterator getEndRequest();
	void setHeader(std::string const &);
	std::string getHeader();
	std::string getFileName();
	void clearRequest();
	void deleteRequest(std::string const &);
	void updateRequest(std::string const &, std::string const &);
	void loadCompleteRequest(const std::string &);
};
