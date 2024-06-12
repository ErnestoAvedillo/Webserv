/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcheel-n <jcheel-n@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 17:38:18 by eavedill          #+#    #+#             */
/*   Updated: 2024/06/12 17:14:56 by jcheel-n         ###   ########.fr       */
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
	varnames[VAR_LOC_CGI_PATH] = 0;
	varnames[VAR_LOC_CGI_EXTENSION] = 0;
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
	locationMethods[VAR_LOC_CGI_PATH] = &Location::setCgiPath;
	locationMethods[VAR_LOC_CGI_EXTENSION] = &Location::setCgiExtension;
	return locationMethods;
}

// Default constructor
Location::Location() 
{
	name = "";
	root = "";
	return_ = "";
	index = "";
	allowMethodsStr = "";
	autoindex = "";
	alias = "";
}
Location::Location(std::string const &content)
{
	std::cout << "Location constructor" << std::endl;
	this->loadData(content);
}
// Copy constructor
Location::Location(const Location& other)
{
	setName(other.name);
	setRoot(other.root);
	setReturn(other.return_);
	setIndex(other.index);
	setAllowMethods(other.allowMethodsStr);
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
const std::string &Location::getAllowMethods() const { return allowMethodsStr; }
const std::string &Location::getAutoindex() const { return autoindex; }
const std::string &Location::getAlias() const { return alias; }

// Setter methods
void Location::setName(const std::string &n) { name = n; }
void Location::setRoot(const std::string &r) { root = r; }
void Location::setReturn(const std::string &ret) { return_ = ret; }
void Location::setIndex(const std::string &idx) { index = idx; }
void Location::setAllowMethods(const std::string &allow) { allowMethodsStr = allow; }
void Location::setAutoindex(const std::string &autoidx) { autoindex = autoidx; }
void Location::setAlias(const std::string &als) { alias = als; }
void Location::setCgiPath(const std::string &cgi_path) { this->cgiPath = cgi_path; }
void Location::setCgiExtension(const std::string &cgi_ext) { this->cgiExtension = cgi_ext; }

// Load data from a string configuration
int Location::loadData(const std::string &content)
{
	std::string start_string = STR_START;
	std::string line;
	std::string straux;
	std::map<std::string, int> varnames = var_names_location();

	std::istringstream fileContentStream(content);
	while (std::getline(fileContentStream, line, ';'))
	{
		line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
		if (line == "}")
			continue;
		std::map<std::string, int>::iterator it = varnames.begin();
		while (it != varnames.end())
		{
			if (line.substr(0 , line.find(":") ) == it->first)
			{
				if (it->second == 1)
					std::cerr << "Error: duplicated variable " << it->first << std::endl;
				it->second = 1;
				break;
			}
			it++;
		}
		if (line.length() == 0 || line == "}" || line == "{")
			continue;
		else if (it == varnames.end())
			std::cerr << "Error: Unrecognized variable " << line.substr(0, line.find(":")) << "$" << std::endl;
		else
		{
			straux = line.substr(line.find(":") + 1, line.size());
			(this->*getLocationMethods()[it->first])(straux);
		}
	}
	return 0;
}

void Location::print()
{
	std::cout << "-------------------- Location --------------------" << std::endl;
	std::cout << "Name: " << name << std::endl;
	std::cout << "Root: " << root << std::endl;
	std::cout << "Return: " << return_ << std::endl;
	std::cout << "Index: " << index << std::endl;
	std::cout << "Allow Methods: " << allowMethodsStr << std::endl;
	std::cout << "Autoindex: " << autoindex << std::endl;
	std::cout << "Alias: " << alias << std::endl;
	std::cout << "Cgi Path: " << cgiPath << std::endl;
	std::cout << "Cgi Extension: " << cgiExtension << std::endl;
}

// location:{                   
//       name:/tours;
//       root:docs/fusion_web;           # root folder of the location, if not specified, taken from the server. 
//                                       # EX: - URI /tours           --> docs/fusion_web/tours
//                                       #     - URI /tours/page.html --> docs/fusion_web/tours/page.html 
//       autoindex:on;                   # turn on/off directory listing
//       allow_methods: POST,GET;         # allowed methods in location, GET only by default
//       index:index.html;               # default page when requesting a directory, copies root index by default
//       return:abc/index1.html;         # redirection
//       alias: docs/fusion_web;         # replaces location part of URI. 
//                                       # EX: - URI /tours           --> docs/fusion_web
//                                       #     - URI /tours/page.html --> docs/fusion_web/page.html 
//   }
void Location::checkVariables()
{
	if (!Parser::checkLocationName(this->getName()))
		exit(1);

	switch (Parser::checkRootAliasReturn(this->root, this->alias,this->return_))
	{
		case 3:
			Parser::checkReturn(this);
			return ;
		default:
			break;
	}
	// {
	// 	printLog("ERROR", "root\t\t<" + root + ">\tnot a valid directory." );
	// 	exit(1);
	// }
	Parser::checkAutoIndex(this->getAutoindex());
	Parser::checkIndex(this->getIndex(), this->getRoot());
	
}