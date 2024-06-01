/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Import.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 08:30:36 by eavedill          #+#    #+#             */
/*   Updated: 2024/04/21 12:36:03 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMPORT_HPP
#define IMPORT_HPP
#include <iostream>
#include <map>
#include <fstream>
#include <limits>

class ImportData
{
private:
	std::map<std::string, std::string> _list_data;
	char _delimiter;

public:
	ImportData();
	ImportData( char const &);
	ImportData(ImportData const &);
	ImportData &operator=(ImportData const &);
	~ImportData();
	std::string getTemplate(std::string &);
	bool readFile(std::string);
	void setDelimiter(char const &);
	char getDelimiter();
	std::map<std::string, std::string>::iterator getBegin();
	std::map<std::string, std::string>::iterator getEnd();
};

#endif
