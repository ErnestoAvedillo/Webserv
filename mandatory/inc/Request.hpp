/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42barcelona>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 17:42:05 by eavedill          #+#    #+#             */
/*   Updated: 2024/04/25 19:45:43 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../inc/main.hpp"
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
