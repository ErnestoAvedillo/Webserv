/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42barcelona>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 19:10:51 by eavedill          #+#    #+#             */
/*   Updated: 2024/05/13 19:05:32 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Header.hpp"

Header::Header()
{
	this->Type = "";
	this->filename.setFilename("");
	this->HTTPvers = "";
}

Header::Header(Header const &src)
{
	*this = src;
}

// Header &Header::operator=(Header const &header)
// {
// 	this->Type = header.Type;
// 	this->filename.setFilename(header.getFileName());
// 	this->HTTPvers = header.HTTPvers;
// 	this->ContentLength = header.ContentLength;
// 	this->filename.getErrCode();
// 	return (*this);
// }

Header::~Header()
{
}

std::string Header::getType()
{
	return (this->Type);
}

std::string Header::getFileName() const
{
	return (this->filename.getFilename());
}

void Header::setFilename(std::string name)
{
	this->filename.setFilename(name);
}

std::string Header::getHTTPvers()
{
	return (this->HTTPvers);
}

int Header::getContentLength()
{
	return (this->ContentLength);
}

void Header::setContentLength(int contentlength)
{
	this->ContentLength = contentlength;
}

void Header::setType(std::string type)
{
	this->Type = type;
}

void Header::setHTTPvers(std::string httpvers)
{
	this->HTTPvers = httpvers;
}

void Header::setHeader(std::string header)
{
	std::cout << "Header parts: " << header << std::endl;
	std::vector<std::string> header_parts = splitString(header, ' ');
	this->setType(header_parts[0]);
	if (header_parts[1] == "/")
		this->setFilename("./www/index.html");
	else
		this->setFilename("./www" + header_parts[1]);
	this->setHTTPvers(header_parts[2]);
}

std::string Header::getHeader()
{
	std::string header = HTTPvers + " " + toString(this->filename.getErrCode()) + " " + this->filename.getErrDesc() + " " + filename.getErrDesc() + "\nContent-Type: " + this->Type + "\nContent-Length: ";
	return (header);
}

void Header::setHeader(Header const &header)
{
	*this = header;
}
