/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42barcelona>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 19:10:51 by eavedill          #+#    #+#             */
/*   Updated: 2024/04/26 15:53:52 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Header.hpp"

Header::Header()
{
	this->Type = "";
	this->FileName.setFilename("");
	this->HTTPvers = "";
}

Header::Header(Header const &src)
{
	*this = src;
}

Header &Header::operator=(Header const &header)
{
	this->Type = header.Type;
	this->FileName.setFilename(header.getFileName());
	this->HTTPvers = header.HTTPvers;
	this->ContentLength = header.ContentLength;
	this->setErrCode();
	return (*this);
}

Header::Header(std::string type, std::string filename, std::string httpvers)
{
	this->Type = type;
	this->FileName = filename;
	this->HTTPvers = httpvers;
}

Header::~Header()
{
}

std::string Header::getType()
{
	return (this->Type);
}

std::string Header::getFileName()
{
	return (this->FileName.getFilename());
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

void Header::setFileName(std::string filename)
{
	this->FileName = filename;
}

void Header::setHTTPvers(std::string httpvers)
{
	this->HTTPvers = httpvers;
}

void Header::setHeader(std::string header)
{
	std::vector<std::string> header_parts = splitString(header, ' ');
	this->setType(header_parts[0]);
	if (header_parts[1] == "/")
		this->setFileName("./www/index.html");
	else
		this->setFileName("./www" + header_parts[1]);
	this->setHTTPvers(header_parts[2]);
}

std::string Header::getHeader()
{
	class ErrCode error;

	std::string header = HTTPvers + " " + itos(this->Err_code) + " " + error.getErrorMessage(this->Err_code) + "\nContent-Type: " + this->Type + "\nContent-Length: ";
	return (header);
}

void Header::setHeader(Header const &header)
{
	*this = header;
}