/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Attributes.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 18:22:26 by eavedill          #+#    #+#             */
/*   Updated: 2024/06/09 12:56:54 by eavedill         ###   ########.fr       */
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
	return std::to_string(this->attributes.st_size);
}	

std::string Attributes::getDevice() 
{
	return std::to_string(this->attributes.st_dev);
}

std::string Attributes::getCreated() 
{
	return std::to_string(this->attributes.st_ctime);
}

std::string Attributes::getModified() 
{
	return std::to_string(this->attributes.st_mtime);
}
