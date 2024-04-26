/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42barcelona>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 19:10:51 by eavedill          #+#    #+#             */
/*   Updated: 2024/04/23 22:55:26 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Header.hpp"

Header::Header(){}

Header::Header (Header const &rhd)
{
	*this = rhd;
}

Header &Header::operator=(Header const &rhd){
	std::map<std::string, std::string>::iterator itb = rhd.getBegin();
	std::map<std::string, std::string>::iterator ite = rhd.getEnd();
	while(itb != ite)
	{
		this->loadkey(itb->first, itb->second);
		itb++;
	}
	return *this;
}

void Header::loadKey(std::string &str1, std::string &str2){
	Request.insert(std::pair<std::string, std::string>(str1,str2));
}

std::string Header::getKey(std::map<std::string, std::string>::iterator it)
{
	return it->first;
}
std::map<std::string, std::string>::iterator Header::getBegin()  {
	return Request.begin();
}
std::map<std::string, std::string>::iterator Header::getEnd()  {
	return Request.end();
}
std::string Header::getValue(std::string const &str) 
{
	return Request[str];
}


