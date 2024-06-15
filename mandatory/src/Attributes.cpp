/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Attributes.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 18:22:26 by eavedill          #+#    #+#             */
/*   Updated: 2024/06/15 14:34:16 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../inc/Attributes.hpp"

Attributes::Attributes() 
{
	this->name = "";
}

Attributes::Attributes(const std::string& filename)
{
	this->name = filename;
	this->setAttributesFromFile(filename);
}

Attributes::~Attributes() {
}

void Attributes::setAttributesFromFile(const std::string& filename) 
{
	this->name = filename;
	if (stat(filename.c_str(), &this->attributes) != 0) {
		throw std::runtime_error("Error: " + filename + " not found");
	}
}

std::string Attributes::getName() 
{
	return this->name;
}



bool Attributes::getIsDir() 
{
	return this->attributes.st_mode & S_IFDIR;
}

std::string Attributes::getSize() 
{
	return toString(this->attributes.st_size);
}	

std::string Attributes::getDevice() 
{
	return toString(this->attributes.st_dev);
}

std::string Attributes::getCreated() 
{
	return toString(this->attributes.st_ctime);
}

std::string Attributes::getModified() 
{
	std::tm *timeInfo = std::gmtime(&this->attributes.st_mtime);

	char buffer[80];
	std::strftime(buffer, sizeof(buffer), "%A, %d-%b-%y %H:%M:%S GMT", timeInfo);
	return buffer;
}
