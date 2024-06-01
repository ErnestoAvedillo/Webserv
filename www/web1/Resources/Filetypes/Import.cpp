/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Import.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 08:36:47 by eavedill          #+#    #+#             */
/*   Updated: 2024/04/21 12:15:38 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Import.hpp"

ImportData::ImportData(){}

ImportData::ImportData(char const &c)
{
	_delimiter = c;
}

ImportData::ImportData(ImportData const &rhs)
{
	*this = rhs;
}

ImportData &ImportData::operator=(ImportData const &rhs)
{
	this->_delimiter = getDelimiter();
	this->_list_data = rhs._list_data;

	return (*this);
}

ImportData::~ImportData(){}

std::string ImportData::getTemplate(std::string &str)
{
	std::map<std::string, std::string>::iterator it = _list_data.find(str);
	if (it != _list_data.end())
		return (it->second);
	return ("text/html");
}


bool ImportData::readFile(std::string filename)
{
	std::ifstream fData;
	std::string line;
	std::string aux;

	fData.open(filename.c_str(), std::ios::in);
	if (!fData)
	{
		std::cerr << "File " << filename << " couldn't be opened." << std::endl;
		return false;
	}

	std::getline(fData, line);
	while (std::getline(fData, line))
	{
		size_t pos1 = line.find(_delimiter,0);
		size_t pos2 = line.find(_delimiter,pos1 + 1) ;
		_list_data.insert(std::pair<std::string, std::string>(line.substr(0, pos1), line.substr(pos1 + 1, pos2 - pos1 - 1)));
	}
	fData.close();
	return true;
}

void ImportData::setDelimiter(char const& c)
{
	_delimiter = c;
}

char ImportData::getDelimiter()
{
	return (_delimiter);
} 

std::map<std::string, std::string>::iterator ImportData::getBegin()
{
	return (_list_data.begin());
}

std::map<std::string, std::string>::iterator ImportData::getEnd()
{
	return (_list_data.end());
}
