/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42barcelona>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 17:38:18 by eavedill          #+#    #+#             */
/*   Updated: 2024/04/29 18:45:45 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Location.hpp"

std::map<std::string, int> var_names_location()
{
	std::map<std::string, int> varnames;
	varnames[VAR_LOC_NAME] = 0;
	varnames[VAR_LOC_ROOT] = 0;
	varnames[VAR_LOC_RETURN] = 0;
	varnames[VAR_LOC_INDEX] = 0;
	varnames[VAR_LOC_ALLOW_METHODS] = 0;
	varnames[VAR_LOC_AUTOINDEX] = 0;
	varnames[VAR_LOC_ALIAS] = 0;
	return varnames;
}
// Method to get the methods of the location
std::map<std::string, void (Location::*)(const std::string&)> getLocationMethods()
{
	std::map<std::string, void (Location::*)(const std::string&)> locationMethods;
	
	locationMethods[VAR_LOC_NAME] = &Location::setName;
	locationMethods[VAR_LOC_ROOT] = &Location::setRoot;
	locationMethods[VAR_LOC_RETURN] = &Location::setReturn;
	locationMethods[VAR_LOC_INDEX] = &Location::setIndex;
	locationMethods[VAR_LOC_ALLOW_METHODS] = &Location::setAllowMethods;
	locationMethods[VAR_LOC_AUTOINDEX] = &Location::setAutoindex;
	locationMethods[VAR_LOC_ALIAS] = &Location::setAlias;
	return locationMethods;
}

// Default constructor
Location::Location() 
{
	name = "";
	root = "";
	return_ = "";
	index = "";
	allow_methods = "";
	autoindex = "";
	alias = "";
}
Location::Location(std::string const &content)
{
	this->loadData(content);
}
// Copy constructor
Location::Location(const Location& other)
{
	setName(other.name);
	setRoot(other.root);
	setReturn(other.return_);
	setIndex(other.index);
	setAllowMethods(other.allow_methods);
	setAutoindex(other.autoindex);
	setAlias(other.alias);
}

	// Destructor
	Location::~Location() {}

	// Getter methods
	const std::string &Location::getName() const { return name; }
	const std::string &Location::getRoot() const { return root; }
	const std::string &Location::getReturn() const { return return_; }
	const std::string &Location::getIndex() const { return index; }
	const std::string &Location::getAllowMethods() const { return allow_methods; }
	const std::string &Location::getAutoindex() const { return autoindex; }
	const std::string &Location::getAlias() const { return alias; }

	// Setter methods
	void Location::setName(const std::string &n) { name = n; }
	void Location::setRoot(const std::string &r) { root = r; }
	void Location::setReturn(const std::string &ret) { return_ = ret; }
	void Location::setIndex(const std::string &idx) { index = idx; }
	void Location::setAllowMethods(const std::string &allow) { allow_methods = allow; }
	void Location::setAutoindex(const std::string &autoidx) { autoindex = autoidx; }
	void Location::setAlias(const std::string &als) { alias = als; }

// Load data from a string configuration
	int Location::loadData(const std::string &content)
	{
		std::string start_string = STR_START;
		std::string line;
		std::string straux;
		std::map<std::string, int> varnames = var_names_location();
		if (std::count(content.begin(), content.end(), '{') - std::count(content.begin(), content.end(), '}') != 0)
		{
			std::cerr << "Error: Llaves no balanceadas" << std::endl;
			return -1;
		}
		if (content.find(start_string) != 0)
		{
			std::cerr << "Error: La configuración de location debe empezar con \"" + start_string + "\"" << std::endl;
			return -1;
		}

		std::istringstream fileContentStream(content.substr(start_string.length(), content.length() - 1));
		while (std::getline(fileContentStream, line, ';'))
		{
			if (line == "}")
				continue;
			std::map<std::string, int>::iterator it = varnames.begin();
			while (it != varnames.end())
			{
				if (line.find(it->first) != std::string::npos)
				{
					if (it->second == 1)
						std::cout << "Error: " << it->first << " ha sido ya asignado." << std::endl;
					it->second = 1;
					break;
				}
				it++;
			}
			if (it == varnames.end())
				std::cout << "Error: Variable no reconocida: " << line.substr(0, line.find(":")) << std::endl;
			else
			{
				straux = line.substr(line.find(":") + 1, line.size());
				(this->*getLocationMethods()[it->first])(straux);
			}
		}
		return 0;
}

